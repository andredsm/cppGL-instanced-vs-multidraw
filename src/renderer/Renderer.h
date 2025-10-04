#pragma once

#include "../ui/UIManager.h"
#include "GeometryRenderer.h"
#include "InstanceManager.h"
#include "OrbitCamera.h"
#include "ShaderManager.h"

// Forward declarations
struct GLFWwindow;

class Renderer {
public:
  bool init(GLFWwindow *window);
  void render();
  void cleanup();
  void handleInput(double deltaTime);
  void onWindowResize(int width, int height);

private:
  // Window reference
  GLFWwindow *_window = nullptr;

  // Modular components
  OrbitCamera _camera;
  InstanceManager _instanceManager;
  ShaderManager _shaderManager;
  GeometryRenderer _geometryRenderer;
  UIManager _uiManager;

  // Helper methods
  bool _initializeComponents();
  void _setupUICallbacks();
  void _setupInputCallbacks();
  void _handleInstanceCountChange(int count);
  void _handleSphereParamsChange(float radius, int segments);
};
