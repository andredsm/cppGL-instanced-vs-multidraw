#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include <GL/glew.h>
#include <string>

class ShaderLoader
{
public:
    static GLuint loadShader(const std::string& filePath, GLenum shaderType);

    static GLuint loadShaderFromSource(const std::string& shaderSource, GLenum shaderType);

    static GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);
};

#endif  // SHADERLOADER_H