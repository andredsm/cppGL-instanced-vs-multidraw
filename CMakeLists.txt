cmake_minimum_required(VERSION 3.15)
project(OpenGLRenderer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)
include_directories(${CMAKE_BINARY_DIR}/imgui_backends)

# Find packages provided by Conan
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)
find_package(glm REQUIRED)
find_package(opengl_system REQUIRED)
find_package(imgui REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/renderer/Renderer.cpp
    src/renderer/Camera.cpp
    src/renderer/OrbitCamera.cpp
    src/renderer/InstanceManager.cpp
    src/renderer/ShaderManager.cpp
    src/renderer/GeometryRenderer.cpp
    src/ui/UIManager.cpp
    src/utils/ShaderLoader.cpp
    src/geo/Sphere.cpp
)

# Add ImGui backend source files
list(APPEND SOURCES
    ${CMAKE_BINARY_DIR}/imgui_backends/imgui_impl_glfw.cpp
    ${CMAKE_BINARY_DIR}/imgui_backends/imgui_impl_opengl3.cpp
)

# Create the executable
add_executable(OpenGLRenderer ${SOURCES})

# Link libraries using Conan targets
target_link_libraries(OpenGLRenderer 
    glfw 
    GLEW::GLEW
    glm::glm
    opengl::opengl
    imgui::imgui
)