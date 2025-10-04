#include "ShaderManager.h"
#include "../shaders/EmbeddedShaders.h"
#include "../utils/ShaderLoader.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

ShaderManager::ShaderManager() : _shaderProgram(0) {}

ShaderManager::~ShaderManager() { cleanup(); }

bool ShaderManager::loadShaders(const std::string &vertexPath,
                                const std::string &fragmentPath) {
  GLuint vertexShader = ShaderLoader::loadShader(vertexPath, GL_VERTEX_SHADER);
  GLuint fragmentShader =
      ShaderLoader::loadShader(fragmentPath, GL_FRAGMENT_SHADER);

  if (vertexShader == 0 || fragmentShader == 0) {
    std::cerr << "Failed to load shaders: " << vertexPath << ", "
              << fragmentPath << std::endl;
    return false;
  }

  _shaderProgram = ShaderLoader::createProgram(vertexShader, fragmentShader);

  if (_shaderProgram == 0) {
    std::cerr << "Failed to create shader program" << std::endl;
    return false;
  }

  return true;
}

bool ShaderManager::loadEmbeddedShaders() {
  GLuint vertexShader = ShaderLoader::loadShaderFromSource(
      EmbeddedShaders::VERTEX_SHADER, GL_VERTEX_SHADER);
  GLuint fragmentShader = ShaderLoader::loadShaderFromSource(
      EmbeddedShaders::FRAGMENT_SHADER, GL_FRAGMENT_SHADER);

  if (vertexShader == 0 || fragmentShader == 0) {
    std::cerr << "Failed to load embedded shaders" << std::endl;
    return false;
  }

  _shaderProgram = ShaderLoader::createProgram(vertexShader, fragmentShader);

  if (_shaderProgram == 0) {
    std::cerr << "Failed to create shader program from embedded shaders"
              << std::endl;
    return false;
  }

  return true;
}

void ShaderManager::useProgram() const {
  if (_shaderProgram != 0) {
    glUseProgram(_shaderProgram);
  }
}

void ShaderManager::cleanup() {
  if (_shaderProgram != 0) {
    glDeleteProgram(_shaderProgram);
    _shaderProgram = 0;
  }
}

void ShaderManager::setMatrix4(const std::string &name,
                               const glm::mat4 &matrix) const {
  int location = _getUniformLocation(name);
  if (location != -1) {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }
}

void ShaderManager::setFloat(const std::string &name, float value) const {
  int location = _getUniformLocation(name);
  if (location != -1) {
    glUniform1f(location, value);
  }
}

void ShaderManager::setInt(const std::string &name, int value) const {
  int location = _getUniformLocation(name);
  if (location != -1) {
    glUniform1i(location, value);
  }
}

void ShaderManager::setVec3(const std::string &name,
                            const glm::vec3 &vector) const {
  int location = _getUniformLocation(name);
  if (location != -1) {
    glUniform3fv(location, 1, glm::value_ptr(vector));
  }
}

int ShaderManager::_getUniformLocation(const std::string &name) const {
  if (_shaderProgram == 0) {
    return -1;
  }

  int location = glGetUniformLocation(_shaderProgram, name.c_str());
  if (location == -1) {
    std::cerr << "Warning: Uniform '" << name << "' not found in shader program"
              << std::endl;
  }

  return location;
}