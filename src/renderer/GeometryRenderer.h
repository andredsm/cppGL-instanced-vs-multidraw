#pragma once

#include <vector>
#include <GL/glew.h>
#include "../geo/Sphere.h"
#include "RenderMethod.h"

// Forward declarations
class InstanceManager;
class Camera;
class ShaderManager;

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

  // Rendering methods
  void renderInstanced(const InstanceManager& instanceManager, const Camera& camera);
  void renderMultiDraw(const InstanceManager& instanceManager, const Camera& camera);
  void renderMultiDrawIndirect(const InstanceManager& instanceManager, const Camera& camera);

  // Set shader manager reference
  void setShaderManager(ShaderManager* shaderManager) {
    _shaderManager = shaderManager;
  }

  // Getters
  const SphereGeometry& getSphereGeometry() const {
    return _sphereGeometry;
  }

  // Render method (placeholder for compatibility)
  void setRenderMethod(RenderMethod method) {
    // No longer needed since we call specific render methods directly
  }

private:
  // Sphere geometry data
  SphereGeometry _sphereGeometry;

  // OpenGL objects
  GLuint _sphereVAO;
  GLuint _sphereVBO;
  GLuint _sphereEBO;
  GLuint _instanceSSBO;    // SSBO for instance matrices
  GLuint _indirectBuffer;  // Buffer for indirect draw commands

  // Reference to shader manager
  ShaderManager* _shaderManager = nullptr;

  // Helper methods
  void _setupVertexAttributes();
  void _setupInstanceSSBO(const InstanceManager& instanceManager);
  void _setupIndirectBuffer(const InstanceManager& instanceManager);
};