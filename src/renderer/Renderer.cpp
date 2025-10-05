#include "Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

bool Renderer::init(GLFWwindow *win) {
  _window = win;

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err)
              << std::endl;
    return false;
  }

  // Set up OpenGL options
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  
  // Get window size for initial viewport and camera setup
  int width, height;
  glfwGetFramebufferSize(_window, &width, &height);
  glViewport(0, 0, width, height);

  // Set clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  // Initialize camera with proper aspect ratio
  float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
  _camera.setPerspective(45.0f, aspectRatio, 0.1f, 100.0f);
  _camera.updateProjectionMatrix();

  // Initialize all components
  if (!_initializeComponents()) {
    std::cerr << "Failed to initialize components" << std::endl;
    return false;
  }

  return true;
}

bool Renderer::_initializeComponents() {
  // Initialize shader manager with embedded shaders
  if (!_shaderManager.loadEmbeddedShaders()) {
    std::cerr << "Failed to load embedded shaders" << std::endl;
    return false;
  }

  // Load multidraw shaders
  if (!_shaderManager.loadMultiDrawShaders()) {
    std::cerr << "Failed to load multidraw shaders" << std::endl;
    return false;
  }

  // Initialize geometry renderer
  if (!_geometryRenderer.initialize()) {
    std::cerr << "Failed to initialize geometry renderer" << std::endl;
    return false;
  }

  // Setup initial sphere geometry
  if (!_geometryRenderer.setupSphereGeometry(0.02f, 16)) {
    std::cerr << "Failed to setup sphere geometry" << std::endl;
    return false;
  }

  // Initialize instance manager
  if (!_instanceManager.initialize()) {
    std::cerr << "Failed to initialize instance manager" << std::endl;
    return false;
  }

  // Bind instance data to geometry
  _geometryRenderer.bindInstanceData(_instanceManager);
  
  // Set shader manager reference for multidraw rendering
  _geometryRenderer.setShaderManager(&_shaderManager);

  // Initialize UI manager
  if (!_uiManager.initialize(_window)) {
    std::cerr << "Failed to initialize UI manager" << std::endl;
    return false;
  }

  // Setup UI callbacks
  _setupUICallbacks();

  // Setup input callbacks
  _setupInputCallbacks();

  return true;
}

void Renderer::_setupUICallbacks() {
  _uiManager.setInstanceCountCallback(
      [this](int count) { _handleInstanceCountChange(count); });

  _uiManager.setSphereParamsCallback([this](float radius, int segments) {
    _handleSphereParamsChange(radius, segments);
  });

  _uiManager.setRenderMethodCallback([this](RenderMethod method) {
    _handleRenderMethodChange(method);
  });

  // Initialize instance count to match UI state
  const UIState &uiState = _uiManager.getUIState();
  _handleInstanceCountChange(uiState.currentInstanceCount);
  _handleRenderMethodChange(uiState.renderMethod);
}

void Renderer::handleInput(double deltaTime) {
  // Handle orbit camera input (this will internally call updateViewMatrix if
  // needed)
  _camera.handleMouseInput(_window, deltaTime);
}

void Renderer::_setupInputCallbacks() {
  // Set up scroll callback for camera zoom
  glfwSetWindowUserPointer(_window, this);

  glfwSetScrollCallback(
      _window, [](GLFWwindow *window, double xoffset, double yoffset) {
        Renderer *renderer =
            static_cast<Renderer *>(glfwGetWindowUserPointer(window));
        if (renderer) {
          renderer->_camera.handleScrollInput(yoffset);
        }
      });
}

void Renderer::_handleInstanceCountChange(int count) {
  _instanceManager.setInstanceCount(count);
  _instanceManager.updateInstanceData();
  _geometryRenderer.bindInstanceData(_instanceManager);

  // Update UI performance info
  _uiManager.updatePerformanceInfo(_geometryRenderer.getSphereGeometry(),
                                   count);
}

void Renderer::_handleSphereParamsChange(float radius, int segments) {
  // Regenerate sphere geometry
  _geometryRenderer.setupSphereGeometry(radius, segments);
  _geometryRenderer.bindInstanceData(_instanceManager);

  // Update UI performance info
  _uiManager.updatePerformanceInfo(_geometryRenderer.getSphereGeometry(),
                                   _instanceManager.getCurrentInstanceCount());
}

void Renderer::_handleRenderMethodChange(RenderMethod method) {
  _geometryRenderer.setRenderMethod(method);
}

void Renderer::render() {

  // Clear screen completely
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start UI frame
  _uiManager.newFrame();

  // Get current render method from UI
  RenderMethod currentMethod = _uiManager.getUIState().renderMethod;

  // Use appropriate shader program
  _shaderManager.useProgram(currentMethod);

  // Set uniforms
  _shaderManager.setMatrix4("view", _camera.getViewMatrix());
  _shaderManager.setMatrix4("projection", _camera.getProjectionMatrix());

  // Render geometry using the selected method
  _geometryRenderer.render(_instanceManager.getCurrentInstanceCount(), &_instanceManager);

  // Render UI
  _uiManager.render();
}
void Renderer::cleanup() {
  // Cleanup all components
  _uiManager.cleanup();
  _geometryRenderer.cleanup();
  _instanceManager.cleanup();
  _shaderManager.cleanup();
}

void Renderer::onWindowResize(int width, int height) {
  // Update camera aspect ratio and projection matrix
  float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
  _camera.setPerspective(_camera.getFov(), aspectRatio, _camera.getNearPlane(), _camera.getFarPlane());
  _camera.updateProjectionMatrix();
}