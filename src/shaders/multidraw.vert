#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

// SSBO for instance matrices
layout(std430, binding = 0) buffer InstanceMatrices {
  mat4 instanceMatrix[];
};

out vec3 fragNormal;
out vec3 fragPosition;

uniform mat4 view;
uniform mat4 projection;

void main() {
  // Get instance matrix using gl_DrawID for multidraw rendering
  mat4 modelMatrix = instanceMatrix[gl_DrawID];

  // Transform position using matrix from SSBO
  vec4 worldPos = modelMatrix * vec4(position, 1.0);
  gl_Position = projection * view * worldPos;

  // Pass data to fragment shader
  fragPosition = worldPos.xyz;
  fragNormal = mat3(modelMatrix) * normal;
}