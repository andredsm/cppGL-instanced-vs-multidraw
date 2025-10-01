#ifndef SPHERE_H
#define SPHERE_H

#include <vector>

struct Vertex
{
    float x, y, z;    // Position
    float nx, ny, nz; // Normal
    float u, v;       // Texture coordinates
};

struct SphereGeometry
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int vertexCount;
    unsigned int indexCount;
};

class Sphere
{
public:
    static SphereGeometry generateSphere(int segments, float radius = 1.0f);

private:
    static void _addVertex(std::vector<Vertex> &vertices, float x, float y, float z, float radius);
};

#endif // SPHERE_H