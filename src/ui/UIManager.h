#pragma once

#include <functional>

// Forward declarations
struct GLFWwindow;
struct SphereGeometry;

// UI callback types
using InstanceCountCallback = std::function<void(int)>;
using SphereParamsCallback = std::function<void(float radius, int segments)>;

struct UIState
{
    bool showUI = true;
    int currentInstanceCount = 10000;
    int maxInstanceCount = 100000;
    float sphereRadius = 0.02f;
    int sphereSegments = 16;

    // Performance info
    unsigned int vertexCount = 0;
    unsigned int triangleCount = 0;
};

class UIManager
{
public:
    UIManager();

    // Initialization and cleanup
    bool initialize(GLFWwindow* window);
    void cleanup();

    // Rendering
    void newFrame();
    void render();

    // State management
    void setUIState(const UIState& state)
    {
        _uiState = state;
    }
    const UIState& getUIState() const
    {
        return _uiState;
    }

    // Callbacks
    void setInstanceCountCallback(InstanceCountCallback callback)
    {
        _onInstanceCountChanged = callback;
    }
    void setSphereParamsCallback(SphereParamsCallback callback)
    {
        _onSphereParamsChanged = callback;
    }

    // Update performance info
    void updatePerformanceInfo(const SphereGeometry& geometry, int instanceCount);

private:
    UIState _uiState;
    GLFWwindow* _window;

    // Callbacks
    InstanceCountCallback _onInstanceCountChanged;
    SphereParamsCallback _onSphereParamsChanged;

    // Helper methods
    void _renderControlPanel();
    void _renderPerformanceInfo();
};