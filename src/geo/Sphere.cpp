#include "Sphere.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

SphereGeometry Sphere::generateSphere(int segments, float radius)
{
    SphereGeometry geometry;

    // Clear vectors
    geometry.vertices.clear();
    geometry.indices.clear();

    // Calculate number of vertices and indices
    int rings = segments;
    int sectors = segments * 2;

    float const R = 1.0f / (float)(rings - 1);
    float const S = 1.0f / (float)(sectors - 1);

    // Generate vertices
    for (int r = 0; r < rings; ++r)
    {
        for (int s = 0; s < sectors; ++s)
        {
            float const y = sin(-M_PI / 2 + M_PI * r * R);
            float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            Vertex vertex;
            // Position
            vertex.x = x * radius;
            vertex.y = y * radius;
            vertex.z = z * radius;

            // Normal (normalized position for a unit sphere)
            vertex.nx = x;
            vertex.ny = y;
            vertex.nz = z;

            // Texture coordinates
            vertex.u = s * S;
            vertex.v = r * R;

            geometry.vertices.push_back(vertex);
        }
    }

    // Generate indices
    for (int r = 0; r < rings - 1; ++r)
    {
        for (int s = 0; s < sectors - 1; ++s)
        {
            int curRow = r * sectors;
            int nextRow = (r + 1) * sectors;

            // First triangle
            geometry.indices.push_back(curRow + s);
            geometry.indices.push_back(nextRow + s);
            geometry.indices.push_back(nextRow + (s + 1));

            // Second triangle
            geometry.indices.push_back(curRow + s);
            geometry.indices.push_back(nextRow + (s + 1));
            geometry.indices.push_back(curRow + (s + 1));
        }
    }

    geometry.vertexCount = geometry.vertices.size();
    geometry.indexCount = geometry.indices.size();

    return geometry;
}

void Sphere::_addVertex(std::vector<Vertex> &vertices, float x, float y, float z, float radius)
{
    Vertex vertex;
    vertex.x = x * radius;
    vertex.y = y * radius;
    vertex.z = z * radius;

    // Normal is the normalized position for a unit sphere
    vertex.nx = x;
    vertex.ny = y;
    vertex.nz = z;

    // Calculate texture coordinates
    vertex.u = atan2(x, z) / (2 * M_PI) + 0.5f;
    vertex.v = asin(y) / M_PI + 0.5f;

    vertices.push_back(vertex);
}