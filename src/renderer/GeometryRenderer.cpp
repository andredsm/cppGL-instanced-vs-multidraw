#include "GeometryRenderer.h"
#include "InstanceManager.h"
#include <GL/glew.h>
#include <iostream>

GeometryRenderer::GeometryRenderer()
    : _sphereVAO(0), _sphereVBO(0), _sphereEBO(0) {}

GeometryRenderer::~GeometryRenderer() { cleanup(); }

bool GeometryRenderer::initialize() {
  // Generate VAO
  glGenVertexArrays(1, &_sphereVAO);
  if (_sphereVAO == 0) {
    std::cerr << "Failed to generate VAO" << std::endl;
    return false;
  }

  // Generate buffers
  glGenBuffers(1, &_sphereVBO);
  glGenBuffers(1, &_sphereEBO);

  if (_sphereVBO == 0 || _sphereEBO == 0) {
    std::cerr << "Failed to generate VBO/EBO" << std::endl;
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
}

bool GeometryRenderer::setupSphereGeometry(float radius, int segments) {
  if (_sphereVAO == 0) {
    std::cerr << "GeometryRenderer not initialized" << std::endl;
    return false;
  }

  // Generate sphere geometry
  _sphereGeometry = Sphere::generateSphere(segments, radius);

  glBindVertexArray(_sphereVAO);

  // Upload vertex data
  glBindBuffer(GL_ARRAY_BUFFER, _sphereVBO);
  glBufferData(GL_ARRAY_BUFFER,
               _sphereGeometry.vertices.size() * sizeof(Vertex),
               _sphereGeometry.vertices.data(), GL_STATIC_DRAW);

  // Upload index data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _sphereEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               _sphereGeometry.indices.size() * sizeof(GLuint),
               _sphereGeometry.indices.data(), GL_STATIC_DRAW);

  // Setup vertex attributes
  _setupVertexAttributes();

  glBindVertexArray(0);

  return true;
}

void GeometryRenderer::bindInstanceData(
    const InstanceManager &instanceManager) {
  if (_sphereVAO == 0) {
    std::cerr << "GeometryRenderer not initialized" << std::endl;
    return;
  }

  glBindVertexArray(_sphereVAO);

  // Bind instance VBO
  glBindBuffer(GL_ARRAY_BUFFER, instanceManager.getInstanceVBO());

  // Setup instance attributes
  _setupInstanceAttributes();

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GeometryRenderer::renderInstances(int instanceCount) const {
  if (_sphereVAO == 0 || instanceCount <= 0) {
    return;
  }

  glBindVertexArray(_sphereVAO);
  glDrawElementsInstanced(GL_TRIANGLES, _sphereGeometry.indexCount,
                          GL_UNSIGNED_INT, 0, instanceCount);
  glBindVertexArray(0);
}
void GeometryRenderer::_setupVertexAttributes() {
  // Position (location 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);

  // Normal (location 1)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void GeometryRenderer::_setupInstanceAttributes() {
  // Instance matrix (mat4 - OpenGL maps this to 4 consecutive attribute
  // locations)
  for (int i = 0; i < 4; ++i) {
    glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (void *)(i * 4 * sizeof(float)));
    glEnableVertexAttribArray(3 + i);
    glVertexAttribDivisor(3 + i, 1); // This makes it instanced
  }
}