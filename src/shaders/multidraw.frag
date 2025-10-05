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
    vec3 baseColor = vec3(0.8, 0.6, 1.0); // Different color for multidraw
    vec3 color = baseColor * (0.3 + 0.7 * diff);
    
    // Add some color variation based on position
    color += sin(fragPosition.x * 5.0) * 0.1;
    color += sin(fragPosition.z * 5.0) * 0.1;

    fragColor = vec4(color, 1.0);
}