#pragma once

#include <string>

namespace EmbeddedShaders {

const std::string VERTEX_SHADER = R"(
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
)";

const std::string FRAGMENT_SHADER = R"(
#version 330 core

in vec3 fragNormal;
in vec3 fragPosition;

out vec4 fragColor;


void main() {
    // Normalize the normal
    vec3 normal = normalize(fragNormal);
    
    // Simple lighting calculation
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Create a color based on position for visual variety
    vec3 baseColor = vec3(0.6, 0.8, 1.0);
    vec3 color = baseColor * (0.3 + 0.7 * diff);
    
    // Add some color variation based on position
    color += sin(fragPosition.x * 5.0) * 0.1;
    color += sin(fragPosition.z * 5.0) * 0.1;

    fragColor = vec4(color, 1.0);
}
)";

} // namespace EmbeddedShaders