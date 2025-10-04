#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

// Instance matrix
layout(location = 3) in mat4 instanceMatrix;

out vec3 fragNormal;
out vec3 fragPosition;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Transform position
    vec4 worldPos = instanceMatrix * vec4(position, 1.0);
    gl_Position = projection * view * worldPos;
    
    // Pass data to fragment shader
    fragPosition = worldPos.xyz;
    fragNormal = mat3(instanceMatrix) * normal;
}