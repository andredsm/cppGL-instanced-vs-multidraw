#include "InstanceManager.h"
#include <GL/glew.h>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

InstanceManager::InstanceManager()
    : _instanceVBO(0), _currentInstanceCount(10000), _maxInstanceCount(100000),
      _gridSpacing(0.1f) {}

InstanceManager::~InstanceManager() { cleanup(); }

bool InstanceManager::initialize(int maxInstances) {
  _maxInstanceCount = maxInstances;

  // Generate instance buffer
  glGenBuffers(1, &_instanceVBO);
  if (_instanceVBO == 0) {
    return false;
  }

  updateInstanceData();
  return true;
}

void InstanceManager::setInstanceCount(int count) {
  if (count > 0 && count <= _maxInstanceCount) {
    _currentInstanceCount = count;
  }
}

void InstanceManager::updateInstanceData() {
  _generateGridPositions();

  // Update GPU buffer
  glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, _instanceMatrices.size() * sizeof(glm::mat4),
               _instanceMatrices.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstanceManager::cleanup() {
  if (_instanceVBO != 0) {
    glDeleteBuffers(1, &_instanceVBO);
    _instanceVBO = 0;
  }
  _instanceMatrices.clear();
}

void InstanceManager::_generateGridPositions() {
  _instanceMatrices.clear();
  _instanceMatrices.reserve(_currentInstanceCount);

  // Calculate grid size based on current instance count
  int gridSize = static_cast<int>(std::sqrt(_currentInstanceCount));
  if (gridSize * gridSize < _currentInstanceCount) {
    gridSize++; // Ensure we have enough grid cells
  }

  float offset = (gridSize - 1) * _gridSpacing * 0.5f;

  for (int i = 0; i < _currentInstanceCount; ++i) {
    int x = i % gridSize;
    int z = i / gridSize;

    glm::mat4 model = glm::mat4(1.0f);

    // Position in grid
    float posX = x * _gridSpacing - offset;
    float posZ = z * _gridSpacing - offset;
    float posY = 0.0f;

    model = glm::translate(model, glm::vec3(posX, posY, posZ));
    _instanceMatrices.push_back(model);
  }
}