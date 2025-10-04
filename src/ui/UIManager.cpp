#include "UIManager.h"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "../geo/Sphere.h"

UIManager::UIManager() : _window(nullptr)
{
}

bool UIManager::initialize(GLFWwindow* win)
{
    _window = win;

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return true;
}

void UIManager::cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::newFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::render()
{
    if (_uiState.showUI)
    {
        _renderControlPanel();
    }

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::updatePerformanceInfo(const SphereGeometry& geometry, int instanceCount)
{
    _uiState.vertexCount = geometry.vertexCount * instanceCount;
    _uiState.triangleCount = (geometry.indexCount / 3) * instanceCount;
}

void UIManager::_renderControlPanel()
{
    ImGui::Begin("Sphere Renderer Controls", &_uiState.showUI);

    // Instance count control
    int oldInstanceCount = _uiState.currentInstanceCount;
    ImGui::SliderInt("Instance Count", &_uiState.currentInstanceCount, 1, _uiState.maxInstanceCount);

    if (_uiState.currentInstanceCount != oldInstanceCount && _onInstanceCountChanged)
    {
        _onInstanceCountChanged(_uiState.currentInstanceCount);
    }

    ImGui::Separator();

    // Sphere properties
    float oldRadius = _uiState.sphereRadius;
    int oldSegments = _uiState.sphereSegments;

    ImGui::SliderFloat("Sphere Radius", &_uiState.sphereRadius, 0.01f, 0.1f, "%.3f");
    ImGui::SliderInt("Sphere Segments", &_uiState.sphereSegments, 8, 32);

    if ((_uiState.sphereRadius != oldRadius || _uiState.sphereSegments != oldSegments) && _onSphereParamsChanged)
    {
        _onSphereParamsChanged(_uiState.sphereRadius, _uiState.sphereSegments);
    }

    ImGui::Separator();

    _renderPerformanceInfo();

    ImGui::End();
}

void UIManager::_renderPerformanceInfo()
{
    ImGui::Text("Performance Info:");
    ImGui::Text("Vertices per sphere: %u", _uiState.vertexCount / _uiState.currentInstanceCount);
    ImGui::Text("Triangles per sphere: %u", _uiState.triangleCount / _uiState.currentInstanceCount);
    ImGui::Text("Total vertices: %u", _uiState.vertexCount);
    ImGui::Text("Total triangles: %u", _uiState.triangleCount);
}