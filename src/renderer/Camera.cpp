#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
    : _position(8.0f, 8.0f, 8.0f),
      _target(0.0f, 0.0f, 0.0f),
      _up(0.0f, 1.0f, 0.0f),
      _fov(45.0f),
      _aspectRatio(1.0f), // Will be updated when window size is known
      _nearPlane(0.1f),
      _farPlane(100.0f),
      _viewMatrix(1.0f),
      _projectionMatrix(1.0f)
{
    updateViewMatrix();
    updateProjectionMatrix();
}

void Camera::setPosition(const glm::vec3& pos)
{
    _position = pos;
}

void Camera::setTarget(const glm::vec3& tgt)
{
    _target = tgt;
}

void Camera::setUp(const glm::vec3& upVector)
{
    _up = upVector;
}

void Camera::setPerspective(float fieldOfView, float aspect, float near, float far)
{
    _fov = fieldOfView;
    _aspectRatio = aspect;
    _nearPlane = near;
    _farPlane = far;
}

void Camera::updateViewMatrix()
{
    _viewMatrix = glm::lookAt(_position, _target, _up);
}

void Camera::updateProjectionMatrix()
{
    _projectionMatrix = glm::perspective(glm::radians(_fov), _aspectRatio, _nearPlane, _farPlane);
}