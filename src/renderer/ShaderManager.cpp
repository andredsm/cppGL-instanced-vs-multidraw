#include "ShaderManager.h"
#include "../utils/ShaderLoader.h"
#include "shaders/basic_fragment.h"
#include "shaders/basic_vertex.h"
#include "shaders/multidraw_fragment.h"
#include "shaders/multidraw_vertex.h"

#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

ShaderManager::ShaderManager() : _instancedProgram(0), _multiDrawProgram(0) {}

ShaderManager::~ShaderManager() {
  cleanup();
}

bool ShaderManager::loadShaders(const std::string& vertexPath, const std::string& fragmentPath) {
  GLuint vertexShader = ShaderLoader::loadShader(vertexPath, GL_VERTEX_SHADER);
  GLuint fragmentShader = ShaderLoader::loadShader(fragmentPath, GL_FRAGMENT_SHADER);

  if (vertexShader == 0 || fragmentShader == 0) {
    std::cerr << "Failed to load shaders: " << vertexPath << ", " << fragmentPath << std::endl;
    return false;
  }

  _instancedProgram = ShaderLoader::createProgram(vertexShader, fragmentShader);

  if (_instancedProgram == 0) {
    std::cerr << "Failed to create shader program" << std::endl;
    return false;
  }

  return true;
}

bool ShaderManager::loadEmbeddedShaders() {
  GLuint vertexShader = ShaderLoader::loadShaderFromSource(GeneratedShaders::BASIC_VERTEX_SHADER, GL_VERTEX_SHADER);
  GLuint fragmentShader = ShaderLoader::loadShaderFromSource(GeneratedShaders::BASIC_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);

  if (vertexShader == 0 || fragmentShader == 0) {
    std::cerr << "Failed to load embedded shaders" << std::endl;
    return false;
  }

  _instancedProgram = ShaderLoader::createProgram(vertexShader, fragmentShader);

  if (_instancedProgram == 0) {
    std::cerr << "Failed to create shader program from embedded shaders" << std::endl;
    return false;
  }

  return true;
}

bool ShaderManager::loadMultiDrawShaders() {
  GLuint vertexShader = ShaderLoader::loadShaderFromSource(GeneratedShaders::MULTIDRAW_VERTEX_SHADER, GL_VERTEX_SHADER);
  GLuint fragmentShader = ShaderLoader::loadShaderFromSource(GeneratedShaders::MULTIDRAW_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);

  if (vertexShader == 0 || fragmentShader == 0) {
    std::cerr << "Failed to load multidraw shaders" << std::endl;
    return false;
  }

  _multiDrawProgram = ShaderLoader::createProgram(vertexShader, fragmentShader);

  if (_multiDrawProgram == 0) {
    std::cerr << "Failed to create multidraw shader program" << std::endl;
    return false;
  }

  return true;
}

void ShaderManager::useProgram(RenderMethod method) const {
  _currentMethod = method;
  unsigned int program = _getCurrentProgram();
  if (program != 0) {
    glUseProgram(program);
  }
}

void ShaderManager::cleanup() {
  if (_instancedProgram != 0) {
    glDeleteProgram(_instancedProgram);
    _instancedProgram = 0;
  }
  if (_multiDrawProgram != 0) {
    glDeleteProgram(_multiDrawProgram);
    _multiDrawProgram = 0;
  }
}

void ShaderManager::setMatrix4(const std::string& name, const glm::mat4& matrix) const {
  int location = _getUniformLocation(name);
  if (location != -1) {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }
}

void ShaderManager::setFloat(const std::string& name, float value) const {
  int location = _getUniformLocation(name);
  if (location != -1) {
    glUniform1f(location, value);
  }
}

void ShaderManager::setInt(const std::string& name, int value) const {
  int location = _getUniformLocation(name);
  if (location != -1) {
    glUniform1i(location, value);
  }
}

void ShaderManager::setVec3(const std::string& name, const glm::vec3& vector) const {
  int location = _getUniformLocation(name);
  if (location != -1) {
    glUniform3fv(location, 1, glm::value_ptr(vector));
  }
}

int ShaderManager::_getUniformLocation(const std::string& name) const {
  unsigned int program = _getCurrentProgram();
  if (program == 0) {
    return -1;
  }

  int location = glGetUniformLocation(program, name.c_str());
  if (location == -1) {
    std::cerr << "Warning: Uniform '" << name << "' not found in shader program" << std::endl;
  }

  return location;
}

unsigned int ShaderManager::_getCurrentProgram() const {
  switch (_currentMethod) {
    case RenderMethod::INSTANCED:
      return _instancedProgram;
    case RenderMethod::MULTIDRAW:
      return _multiDrawProgram;
    default:
      return _instancedProgram;
  }
}

unsigned int ShaderManager::getProgram(RenderMethod method) const {
  switch (method) {
    case RenderMethod::INSTANCED:
      return _instancedProgram;
    case RenderMethod::MULTIDRAW:
      return _multiDrawProgram;
    default:
      return _instancedProgram;
  }
}