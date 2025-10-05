#pragma once

#include <string>
#include <glm/glm.hpp>
#include "RenderMethod.h"

class ShaderManager {
public:
  ShaderManager();
  ~ShaderManager();

  // Shader management
  bool loadShaders(const std::string& vertexPath, const std::string& fragmentPath);
  bool loadEmbeddedShaders();
  bool loadMultiDrawShaders();
  void useProgram(RenderMethod method = RenderMethod::INSTANCED) const;
  void cleanup();

  // Uniform setters
  void setMatrix4(const std::string& name, const glm::mat4& matrix) const;
  void setFloat(const std::string& name, float value) const;
  void setInt(const std::string& name, int value) const;
  void setVec3(const std::string& name, const glm::vec3& vector) const;

  // Getters
  unsigned int getProgram(RenderMethod method = RenderMethod::INSTANCED) const;

private:
  unsigned int _instancedProgram;
  unsigned int _multiDrawProgram;
  mutable RenderMethod _currentMethod = RenderMethod::INSTANCED;

  // Helper methods
  int _getUniformLocation(const std::string& name) const;
  unsigned int _getCurrentProgram() const;
};