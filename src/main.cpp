#include "renderer/Renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Renderer", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Create the renderer
    Renderer renderer;
    if (!renderer.init(window))
    {
        std::cerr << "Failed to initialize the renderer" << std::endl;
        return -1;
    }

    // FPS calculation variables
    double lastTime = glfwGetTime();
    int frameCount = 0;
    double fps = 0.0;

    // Main rendering loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate FPS
        double currentTime = glfwGetTime();
        frameCount++;

        // Update FPS every second
        if (currentTime - lastTime >= 1.0)
        {
            fps = frameCount / (currentTime - lastTime);
            frameCount = 0;
            lastTime = currentTime;

            // Update window title with FPS
            std::ostringstream title;
            title << "OpenGL Renderer - FPS: " << std::fixed << std::setprecision(1) << fps;
            glfwSetWindowTitle(window, title.str().c_str());
        }

        renderer.render();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Cleanup
    renderer.cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}