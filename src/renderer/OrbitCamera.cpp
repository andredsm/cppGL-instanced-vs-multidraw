#include "OrbitCamera.h"
#include <algorithm>
#include <cmath>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

OrbitCamera::OrbitCamera()
    : Camera(), _orbitCenter(0.0f, 0.0f, 0.0f), _distance(10.0f),
      _azimuth(0.0f), _elevation(0.0f), _minDistance(1.0f), _maxDistance(50.0f),
      _minElevation(-glm::pi<float>() * 0.49f), // Just under 90 degrees
      _maxElevation(glm::pi<float>() * 0.49f),  // Just under 90 degrees
      _rotationSensitivity(0.005f), _zoomSensitivity(1.0f), _enabled(true),
      _isMousePressed(false), _lastMouseX(0.0), _lastMouseY(0.0) {
  // Initialize with a good default position
  setAngles(glm::radians(45.0f), glm::radians(30.0f));
  updateOrbitPosition();
}

void OrbitCamera::setOrbitCenter(const glm::vec3 &center) {
  _orbitCenter = center;
  updateOrbitPosition();
}

void OrbitCamera::setDistance(float distance) {
  _distance = distance;
  _clampDistance();
  updateOrbitPosition();
}

void OrbitCamera::setAngles(float azimuth, float elevation) {
  _azimuth = azimuth;
  _elevation = elevation;
  _clampAngles();
  updateOrbitPosition();
}

void OrbitCamera::setZoomRange(float minDistance, float maxDistance) {
  _minDistance = std::max(0.1f, minDistance);
  _maxDistance = std::max(_minDistance, maxDistance);
  _clampDistance();
}

void OrbitCamera::setSensitivity(float rotationSensitivity,
                                 float zoomSensitivity) {
  _rotationSensitivity = rotationSensitivity;
  _zoomSensitivity = zoomSensitivity;
}

void OrbitCamera::handleMouseInput(GLFWwindow *window, double deltaTime) {
  if (!_enabled)
    return;

  // Get current mouse position
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);

  // Check if mouse button is pressed
  bool isPressed =
      glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

  if (isPressed && !_isMousePressed) {
    // Just started pressing
    _isMousePressed = true;
    _lastMouseX = mouseX;
    _lastMouseY = mouseY;
  } else if (!isPressed && _isMousePressed) {
    // Just released
    _isMousePressed = false;
  } else if (isPressed && _isMousePressed) {
    // Currently dragging
    double deltaX = mouseX - _lastMouseX;
    double deltaY = mouseY - _lastMouseY;

    // Update angles based on mouse movement
    _azimuth -= deltaX * _rotationSensitivity;
    _elevation += deltaY * _rotationSensitivity;

    _clampAngles();
    updateOrbitPosition();

    _lastMouseX = mouseX;
    _lastMouseY = mouseY;
  }
}

void OrbitCamera::handleScrollInput(double yOffset) {
  if (!_enabled)
    return;

  // Zoom in/out by adjusting distance
  float zoomFactor = 1.0f + (yOffset * _zoomSensitivity * 0.1f);
  _distance /= zoomFactor;

  _clampDistance();
  updateOrbitPosition();
}

void OrbitCamera::updateOrbitPosition() {
  glm::vec3 newPosition = _calculatePosition();
  setPosition(newPosition);
  setTarget(_orbitCenter);
  updateViewMatrix();
}

void OrbitCamera::_clampAngles() {
  // Normalize azimuth to [0, 2π)
  while (_azimuth < 0.0f)
    _azimuth += 2.0f * glm::pi<float>();
  while (_azimuth >= 2.0f * glm::pi<float>())
    _azimuth -= 2.0f * glm::pi<float>();

  // Clamp elevation to prevent gimbal lock
  _elevation = std::clamp(_elevation, _minElevation, _maxElevation);
}

void OrbitCamera::_clampDistance() {
  _distance = std::clamp(_distance, _minDistance, _maxDistance);
}

glm::vec3 OrbitCamera::_calculatePosition() const {
  // Convert spherical coordinates to cartesian
  float x = _distance * cos(_elevation) * cos(_azimuth);
  float y = _distance * sin(_elevation);
  float z = _distance * cos(_elevation) * sin(_azimuth);

  return _orbitCenter + glm::vec3(x, y, z);
}