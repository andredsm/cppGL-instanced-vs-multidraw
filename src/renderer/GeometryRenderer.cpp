#include "GeometryRenderer.h"

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "InstanceManager.h"
#include "ShaderManager.h"

GeometryRenderer::GeometryRenderer() : _sphereVAO(0), _sphereVBO(0), _sphereEBO(0), _instanceSSBO(0) {}

GeometryRenderer::~GeometryRenderer() {
  cleanup();
}

bool GeometryRenderer::initialize() {
  // Generate VAOs
  glGenVertexArrays(1, &_sphereVAO);
  if (_sphereVAO == 0) {
    std::cerr << "Failed to generate VAO" << std::endl;
    return false;
  }

  // Generate buffers
  glGenBuffers(1, &_sphereVBO);
  glGenBuffers(1, &_sphereEBO);
  glGenBuffers(1, &_instanceSSBO);

  if (_sphereVBO == 0 || _sphereEBO == 0 || _instanceSSBO == 0) {
    std::cerr << "Failed to generate VBO/EBO/SSBO" << std::endl;
    cleanup();
    return false;
  }

  return true;
}

void GeometryRenderer::cleanup() {
  if (_sphereVAO != 0) {
    glDeleteVertexArrays(1, &_sphereVAO);
    _sphereVAO = 0;
  }
  if (_sphereVBO != 0) {
    glDeleteBuffers(1, &_sphereVBO);
    _sphereVBO = 0;
  }
  if (_sphereEBO != 0) {
    glDeleteBuffers(1, &_sphereEBO);
    _sphereEBO = 0;
  }
  if (_instanceSSBO != 0) {
    glDeleteBuffers(1, &_instanceSSBO);
    _instanceSSBO = 0;
  }
}

bool GeometryRenderer::setupSphereGeometry(float radius, int segments) {
  if (_sphereVAO == 0) {
    std::cerr << "GeometryRenderer not initialized" << std::endl;
    return false;
  }

  // Generate sphere geometry
  _sphereGeometry = Sphere::generateSphere(segments, radius);

  // Setup instanced VAO
  glBindVertexArray(_sphereVAO);

  // Upload vertex data
  glBindBuffer(GL_ARRAY_BUFFER, _sphereVBO);
  glBufferData(GL_ARRAY_BUFFER, _sphereGeometry.vertices.size() * sizeof(Vertex), _sphereGeometry.vertices.data(), GL_STATIC_DRAW);

  // Upload index data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _sphereEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, _sphereGeometry.indices.size() * sizeof(GLuint), _sphereGeometry.indices.data(), GL_STATIC_DRAW);

  // Setup vertex attributes
  _setupVertexAttributes();

  glBindVertexArray(0);

  return true;
}

void GeometryRenderer::bindInstanceData(const InstanceManager& instanceManager) {
  if (_sphereVAO == 0) {
    std::cerr << "GeometryRenderer not initialized" << std::endl;
    return;
  }

  // Setup SSBO with instance data
  _setupInstanceSSBO(instanceManager);
}

void GeometryRenderer::render(int instanceCount, const InstanceManager* instanceManager) const {
  switch (_renderMethod) {
    case RenderMethod::INSTANCED:
      renderInstances(instanceCount);
      break;
    case RenderMethod::MULTIDRAW:
      renderMultiDraw(instanceCount, instanceManager);
      break;
  }
}

void GeometryRenderer::renderInstances(int instanceCount) const {
  if (_sphereVAO == 0 || instanceCount <= 0) {
    return;
  }

  glBindVertexArray(_sphereVAO);
  glDrawElementsInstanced(GL_TRIANGLES, _sphereGeometry.indexCount, GL_UNSIGNED_INT, 0, instanceCount);
  glBindVertexArray(0);
}

void GeometryRenderer::renderMultiDraw(int instanceCount, const InstanceManager* instanceManager) const {
  if (_sphereVAO == 0 || instanceCount <= 0 || !instanceManager) {
    return;
  }

  // Bind the VAO and render with glMultiDrawElements
  // Each draw call will use gl_DrawID to index into the SSBO
  glBindVertexArray(_sphereVAO);

  // Create arrays for multidraw - each draw uses the same geometry but different matrix from SSBO
  std::vector<GLsizei> counts(instanceCount, _sphereGeometry.indexCount);
  std::vector<const void*> indices(instanceCount, 0);  // All use same index buffer

  // Render all spheres in one multidraw call
  // gl_DrawID will be 0, 1, 2, ... for each draw call
  glMultiDrawElements(GL_TRIANGLES, counts.data(), GL_UNSIGNED_INT, indices.data(), instanceCount);

  glBindVertexArray(0);
}
void GeometryRenderer::_setupVertexAttributes() {
  // Position (location 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);

  // Normal (location 1)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void GeometryRenderer::_setupInstanceSSBO(const InstanceManager& instanceManager) {
  // Bind SSBO and upload instance matrices
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, _instanceSSBO);

  const auto& matrices = instanceManager.getInstanceMatrices();
  glBufferData(GL_SHADER_STORAGE_BUFFER, matrices.size() * sizeof(glm::mat4), matrices.data(), GL_DYNAMIC_DRAW);

  // Bind to binding point 0
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _instanceSSBO);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
