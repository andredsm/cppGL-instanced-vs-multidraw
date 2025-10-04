#pragma once

#include <glm/glm.hpp>
#include <string>

class ShaderManager {
public:
  ShaderManager();
  ~ShaderManager();

  // Shader management
  bool loadShaders(const std::string &vertexPath,
                   const std::string &fragmentPath);
  bool loadEmbeddedShaders();
  void useProgram() const;
  void cleanup();

  // Uniform setters
  void setMatrix4(const std::string &name, const glm::mat4 &matrix) const;
  void setFloat(const std::string &name, float value) const;
  void setInt(const std::string &name, int value) const;
  void setVec3(const std::string &name, const glm::vec3 &vector) const;

  // Getters
  unsigned int getProgram() const { return _shaderProgram; }
  bool isValid() const { return _shaderProgram != 0; }

private:
  unsigned int _shaderProgram;

  // Helper methods
  int _getUniformLocation(const std::string &name) const;
};