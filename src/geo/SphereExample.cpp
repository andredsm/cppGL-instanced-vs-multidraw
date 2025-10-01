#include "geo/Sphere.h"
#include <iostream>

/*
 * Example usage of the Sphere geometry generator
 *
 * This demonstrates how to create sphere geometry and use it with OpenGL
 */

void exampleUsage()
{
    // Generate a sphere with 20 segments (moderate resolution)
    SphereGeometry sphere = Sphere::generateSphere(20, 1.0f);

    std::cout << "Generated sphere with:" << std::endl;
    std::cout << "  Vertices: " << sphere.vertexCount << std::endl;
    std::cout << "  Indices: " << sphere.indexCount << std::endl;
    std::cout << "  Triangles: " << sphere.indexCount / 3 << std::endl;

    // Example OpenGL buffer creation
    GLuint VBO, EBO, VAO;

    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sphere.vertices.size() * sizeof(Vertex),
                 sphere.vertices.data(),
                 GL_STATIC_DRAW);

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sphere.indices.size() * sizeof(GLuint),
                 sphere.indices.data(),
                 GL_STATIC_DRAW);

    // Setup vertex attributes
    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);

    // Normal attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind VAO
    glBindVertexArray(0);

    // To render the sphere:
    // glBindVertexArray(VAO);
    // glDrawElements(GL_TRIANGLES, sphere.indexCount, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0);

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}