#pragma once

#include <glm/glm.hpp>
#include <vector>

class InstanceManager
{
public:
    InstanceManager();
    ~InstanceManager();

    // Instance management
    bool initialize(int maxInstances = 100000);
    void setInstanceCount(int count);
    void updateInstanceData();
    void cleanup();

    // Getters
    int getCurrentInstanceCount() const
    {
        return _currentInstanceCount;
    }
    int getMaxInstanceCount() const
    {
        return _maxInstanceCount;
    }
    unsigned int getInstanceVBO() const
    {
        return _instanceVBO;
    }
    const std::vector<glm::mat4>& getInstanceMatrices() const
    {
        return _instanceMatrices;
    }

    // Grid configuration
    void setSpacing(float spacing)
    {
        _gridSpacing = spacing;
    }
    float getSpacing() const
    {
        return _gridSpacing;
    }

private:
    // Instance data
    std::vector<glm::mat4> _instanceMatrices;
    unsigned int _instanceVBO;
    int _currentInstanceCount;
    int _maxInstanceCount;

    // Grid configuration
    float _gridSpacing;

    // Helper methods
    void _generateGridPositions();
};