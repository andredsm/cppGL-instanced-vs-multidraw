#include "Renderer.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../utils/ShaderLoader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <iostream>

Renderer::Renderer() : window(nullptr), sphereVAO(0), sphereVBO(0), sphereEBO(0), instanceVBO(0), shaderProgram(0)
{
}

Renderer::~Renderer()
{
    cleanup();
}

bool Renderer::init(GLFWwindow *win)
{
    window = win;

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return false;
    }

    // Set up OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glViewport(0, 0, 800, 600);

    // Set clear color
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Setup components
    if (!_loadShaders())
    {
        std::cerr << "Failed to load shaders" << std::endl;
        return false;
    }

    if (!_setupSphereGeometry())
    {
        std::cerr << "Failed to setup sphere geometry" << std::endl;
        return false;
    }

    if (!_setupInstanceData())
    {
        std::cerr << "Failed to setup instance data" << std::endl;
        return false;
    }

    _updateCamera();

    return true;
}

bool Renderer::_setupSphereGeometry()
{
    // Generate sphere geometry with current settings
    sphereGeometry = Sphere::generateSphere(sphereSegments, sphereRadius);

    // Generate and bind VAO
    glGenVertexArrays(1, &sphereVAO);
    glBindVertexArray(sphereVAO);

    // Create and bind vertex buffer
    glGenBuffers(1, &sphereVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sphereGeometry.vertices.size() * sizeof(Vertex),
                 sphereGeometry.vertices.data(),
                 GL_STATIC_DRAW);

    // Create and bind element buffer
    glGenBuffers(1, &sphereEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sphereGeometry.indices.size() * sizeof(GLuint),
                 sphereGeometry.indices.data(),
                 GL_STATIC_DRAW);

    // Setup vertex attributes
    // Position (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);

    // Normal (location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinates (location 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    return true;
}

bool Renderer::_setupInstanceData()
{
    _updateInstanceData();
    return true;
}

void Renderer::_updateInstanceData()
{
    instanceMatrices.clear();
    instanceMatrices.reserve(maxInstanceCount);

    // Calculate grid size based on current instance count
    int gridSize = static_cast<int>(std::sqrt(currentInstanceCount));
    if (gridSize * gridSize < currentInstanceCount)
    {
        gridSize++; // Ensure we have enough grid cells
    }

    float spacing = 0.1f;
    float offset = (gridSize - 1) * spacing * 0.5f;

    for (int i = 0; i < currentInstanceCount; ++i)
    {
        int x = i % gridSize;
        int z = i / gridSize;
        glm::mat4 model = glm::mat4(1.0f);

        // Position in grid
        float posX = x * spacing - offset;
        float posZ = z * spacing - offset;
        float posY = 0.0f;

        model = glm::translate(model, glm::vec3(posX, posY, posZ));
        instanceMatrices.push_back(model);
    }

    // Update or create instance buffer
    if (instanceVBO == 0)
    {
        glGenBuffers(1, &instanceVBO);
    }

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 instanceMatrices.size() * sizeof(glm::mat4),
                 instanceMatrices.data(),
                 GL_DYNAMIC_DRAW); // Use DYNAMIC_DRAW for changing data

    // Bind to sphere VAO and setup instance attributes
    glBindVertexArray(sphereVAO);

    // Instance matrix (4 vec4s for mat4)
    for (int i = 0; i < 4; ++i)
    {
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                              (void *)(i * 4 * sizeof(float)));
        glEnableVertexAttribArray(3 + i);
        glVertexAttribDivisor(3 + i, 1); // This makes it instanced
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool Renderer::_loadShaders()
{
    GLuint vertexShader = ShaderLoader::loadShader("src/shaders/vertex.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = ShaderLoader::loadShader("src/shaders/fragment.glsl", GL_FRAGMENT_SHADER);

    if (vertexShader == 0 || fragmentShader == 0)
    {
        return false;
    }

    shaderProgram = ShaderLoader::createProgram(vertexShader, fragmentShader);

    return shaderProgram != 0;
}

void Renderer::_updateCamera()
{
    // Setup view matrix (camera looking at the center of the sphere grid)
    glm::vec3 cameraPos = glm::vec3(8.0f, 8.0f, 8.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    viewMatrix = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    // Setup projection matrix
    projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}

void Renderer::render()
{
    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Render UI
    _renderUI();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use shader program
    glUseProgram(shaderProgram);

    // Set view and projection matrices
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    GLint timeLoc = glGetUniformLocation(shaderProgram, "time");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniform1f(timeLoc, glfwGetTime());

    // Bind sphere VAO and render instances
    glBindVertexArray(sphereVAO);
    glDrawElementsInstanced(GL_TRIANGLES, sphereGeometry.indexCount,
                            GL_UNSIGNED_INT, 0, currentInstanceCount);
    glBindVertexArray(0);

    glUseProgram(0);

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::cleanup()
{
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (sphereVAO)
    {
        glDeleteVertexArrays(1, &sphereVAO);
        sphereVAO = 0;
    }
    if (sphereVBO)
    {
        glDeleteBuffers(1, &sphereVBO);
        sphereVBO = 0;
    }
    if (sphereEBO)
    {
        glDeleteBuffers(1, &sphereEBO);
        sphereEBO = 0;
    }
    if (instanceVBO)
    {
        glDeleteBuffers(1, &instanceVBO);
        instanceVBO = 0;
    }
    if (shaderProgram)
    {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }
}

void Renderer::_renderUI()
{
    if (!showUI)
        return;

    ImGui::Begin("Sphere Renderer Controls", &showUI);

    // Instance count control
    int oldInstanceCount = currentInstanceCount;
    ImGui::SliderInt("Instance Count", &currentInstanceCount, 1, maxInstanceCount);

    if (currentInstanceCount != oldInstanceCount)
    {
        _updateInstanceData();
    }

    ImGui::Separator();

    // Sphere properties
    float oldRadius = sphereRadius;
    int oldSegments = sphereSegments;

    ImGui::SliderFloat("Sphere Radius", &sphereRadius, 0.01f, 0.1f, "%.3f");
    ImGui::SliderInt("Sphere Segments", &sphereSegments, 8, 32);

    if (sphereRadius != oldRadius || sphereSegments != oldSegments)
    {
        // Regenerate sphere geometry
        sphereGeometry = Sphere::generateSphere(sphereSegments, sphereRadius);

        // Update VBO with new geometry
        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glBufferData(GL_ARRAY_BUFFER,
                     sphereGeometry.vertices.size() * sizeof(Vertex),
                     sphereGeometry.vertices.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sphereGeometry.indices.size() * sizeof(GLuint),
                     sphereGeometry.indices.data(),
                     GL_STATIC_DRAW);
    }

    ImGui::Separator();

    // Performance info
    ImGui::Text("Performance Info:");
    ImGui::Text("Vertices per sphere: %u", sphereGeometry.vertexCount);
    ImGui::Text("Triangles per sphere: %u", sphereGeometry.indexCount / 3);
    ImGui::Text("Total vertices: %u", sphereGeometry.vertexCount * currentInstanceCount);
    ImGui::Text("Total triangles: %u", (sphereGeometry.indexCount / 3) * currentInstanceCount);

    ImGui::End();
}