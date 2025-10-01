#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

// Instance matrix (4 vec4s)
layout(location = 3) in vec4 instanceMatrix0;
layout(location = 4) in vec4 instanceMatrix1;
layout(location = 5) in vec4 instanceMatrix2;
layout(location = 6) in vec4 instanceMatrix3;

out vec3 fragNormal;
out vec3 fragPosition;
out vec2 fragTexCoord;

uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
    // Reconstruct instance matrix
    mat4 instanceMatrix = mat4(instanceMatrix0, instanceMatrix1, instanceMatrix2, instanceMatrix3);
    
    // Add some animation - optional wave effect
    vec3 animatedPosition = position;
    float wave = sin(time * 2.0 + instanceMatrix[3][0] * 10.0 + instanceMatrix[3][2] * 10.0) * 0.01;
    animatedPosition.y += wave;
    
    // Transform position
    vec4 worldPos = instanceMatrix * vec4(animatedPosition, 1.0);
    gl_Position = projection * view * worldPos;
    
    // Pass data to fragment shader
    fragPosition = worldPos.xyz;
    fragNormal = mat3(instanceMatrix) * normal;
    fragTexCoord = texCoord;
}