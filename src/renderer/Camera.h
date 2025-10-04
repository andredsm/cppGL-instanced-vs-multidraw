#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera();
    ~Camera() = default;

    // Camera setup
    void setPosition(const glm::vec3& position);
    void setTarget(const glm::vec3& target);
    void setUp(const glm::vec3& up);
    void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);

    // Getters
    const glm::mat4& getViewMatrix() const
    {
        return _viewMatrix;
    }
    const glm::mat4& getProjectionMatrix() const
    {
        return _projectionMatrix;
    }
    const glm::vec3& getPosition() const
    {
        return _position;
    }
    const glm::vec3& getTarget() const
    {
        return _target;
    }
    float getFov() const
    {
        return _fov;
    }
    float getAspectRatio() const
    {
        return _aspectRatio;
    }
    float getNearPlane() const
    {
        return _nearPlane;
    }
    float getFarPlane() const
    {
        return _farPlane;
    }

    // Update matrices
    void updateViewMatrix();
    void updateProjectionMatrix();

private:
    // Camera properties
    glm::vec3 _position;
    glm::vec3 _target;
    glm::vec3 _up;

    // Projection properties
    float _fov;
    float _aspectRatio;
    float _nearPlane;
    float _farPlane;

    // Matrices
    glm::mat4 _viewMatrix;
    glm::mat4 _projectionMatrix;
};