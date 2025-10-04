#include "renderer/Renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// Global variables for window resize handling
static int g_windowWidth = 800;
static int g_windowHeight = 600;
static Renderer* g_renderer = nullptr;

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

static void window_resize_callback(GLFWwindow* window, int width, int height) {
  g_windowWidth = width;
  g_windowHeight = height;
  
  // Update viewport
  glViewport(0, 0, width, height);
  
  // Debug output
  std::cout << "Window resized to: " << width << "x" << height << std::endl;
  
  // Update renderer with new dimensions
  if (g_renderer) {
    g_renderer->onWindowResize(width, height);
  }
}

int main() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwSetErrorCallback(glfw_error_callback);

  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  // Create a windowed mode window and its OpenGL context
  GLFWwindow *window =
      glfwCreateWindow(800, 600, "OpenGL Renderer", nullptr, nullptr);

  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Enable V-Sync to prevent tearing
  glfwSwapInterval(1);

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  // Create the renderer
  Renderer renderer;
  g_renderer = &renderer;
  if (!renderer.init(window)) {
    std::cerr << "Failed to initialize the renderer" << std::endl;
    return -1;
  }

  // Set window resize callback
  glfwSetFramebufferSizeCallback(window, window_resize_callback);

  // FPS calculation variables
  double lastTime = glfwGetTime();
  int frameCount = 0;
  double fps = 0.0;
  double lastFrameTime = lastTime;

  // Main rendering loop
  while (!glfwWindowShouldClose(window)) {
    // Calculate FPS and delta time
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    frameCount++;

    // Update FPS every second
    if (currentTime - lastTime >= 1.0) {
      fps = frameCount / (currentTime - lastTime);
      frameCount = 0;
      lastTime = currentTime;

      // Update window title with FPS
      std::ostringstream title;
      title << "OpenGL Renderer - FPS: " << std::fixed << std::setprecision(1)
            << fps;
      glfwSetWindowTitle(window, title.str().c_str());
    }

    // Handle input
    renderer.handleInput(deltaTime);

    renderer.render();

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }

  // Cleanup
  g_renderer = nullptr;
  renderer.cleanup();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}