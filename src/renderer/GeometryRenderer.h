#pragma once

#include <vector>
#include <GL/glew.h>
#include "../geo/Sphere.h"
#include "RenderMethod.h"

// Forward declarations
class InstanceManager;

class GeometryRenderer {
public:
  GeometryRenderer();
  ~GeometryRenderer();

  // Initialization and cleanup
  bool initialize();
  void cleanup();

  // Geometry management
  bool setupSphereGeometry(float radius, int segments);
  void bindInstanceData(const InstanceManager& instanceManager);

  // Rendering
  void render(int instanceCount, const InstanceManager* instanceManager = nullptr) const;
  void renderInstances(int instanceCount) const;
  void renderMultiDraw(int instanceCount, const InstanceManager* instanceManager) const;

  // Set rendering method
  void setRenderMethod(RenderMethod method) {
    _renderMethod = method;
  }

  // Set shader manager reference for multidraw rendering
  void setShaderManager(class ShaderManager* shaderManager) {
    _shaderManager = shaderManager;
  }

  // Setup VAO for different rendering methods
  void setupRenderMethod();

  // Getters
  const SphereGeometry& getSphereGeometry() const {
    return _sphereGeometry;
  }

  bool isInitialized() const {
    return _sphereVAO != 0;
  }

private:
  // Sphere geometry data
  SphereGeometry _sphereGeometry;

  // OpenGL objects
  unsigned int _sphereVAO;
  unsigned int _sphereVBO;
  unsigned int _sphereEBO;
  unsigned int _instanceSSBO;  // SSBO for instance matrices

  // Rendering method
  RenderMethod _renderMethod = RenderMethod::INSTANCED;

  // Reference to shader manager for multidraw rendering
  class ShaderManager* _shaderManager = nullptr;

  // Helper methods
  void _setupVertexAttributes();
  void _setupInstanceSSBO(const InstanceManager& instanceManager);
};