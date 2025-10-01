#pragma once

#include "../geo/Sphere.h"

#include <glm/gtc/type_ptr.hpp>
#include <vector>

// Forward declarations
struct GLFWwindow;
struct SphereGeometry;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool init(GLFWwindow *window);
    void render();
    void cleanup();

private:
    // Window reference
    GLFWwindow *window;

    // Sphere geometry
    SphereGeometry sphereGeometry;
    unsigned int sphereVAO, sphereVBO, sphereEBO;

    // Instance data
    unsigned int instanceVBO;
    std::vector<glm::mat4> instanceMatrices;
    int currentInstanceCount = 10000;
    int maxInstanceCount = 100000; // Maximum supported instances

    // Shader program
    unsigned int shaderProgram;

    // Camera matrices
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    // UI state
    bool showUI = true;
    float sphereRadius = 0.02f;
    int sphereSegments = 16;

    // Helper methods
    bool _setupSphereGeometry();
    bool _setupInstanceData();
    bool _loadShaders();
    void _updateCamera();
    void _updateInstanceData();
    void _renderUI();
};
