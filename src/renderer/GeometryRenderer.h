#pragma once

#include "../geo/Sphere.h"

// Forward declarations
class InstanceManager;

class GeometryRenderer
{
public:
    GeometryRenderer();
    ~GeometryRenderer();

    // Initialization and cleanup
    bool initialize();
    void cleanup();

    // Geometry management
    bool setupSphereGeometry(float radius, int segments);
    void bindInstanceData(const InstanceManager& instanceManager);

    // Rendering
    void renderInstances(int instanceCount) const;

    // Getters
    const SphereGeometry& getSphereGeometry() const
    {
        return _sphereGeometry;
    }
    bool isInitialized() const
    {
        return _sphereVAO != 0;
    }

private:
    // Sphere geometry data
    SphereGeometry _sphereGeometry;

    // OpenGL objects
    unsigned int _sphereVAO;
    unsigned int _sphereVBO;
    unsigned int _sphereEBO;

    // Helper methods
    bool _createBuffers();
    void _setupVertexAttributes();
    void _setupInstanceAttributes();
};