cmake_minimum_required(VERSION 3.15)
project(OpenGLRenderer)

# Include Conan toolchain
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

# Include directories
include_directories(include)

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
    src/utils/ShaderLoader.cpp
    src/geo/Sphere.cpp
    src/imgui/imgui_impl_glfw.cpp
    src/imgui/imgui_impl_opengl3.cpp
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