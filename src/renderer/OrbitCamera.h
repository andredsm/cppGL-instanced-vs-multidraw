#pragma once

#include "Camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class OrbitCamera : public Camera {
public:
  OrbitCamera();
  ~OrbitCamera() = default;

  // Orbit camera specific setup
  void setOrbitCenter(const glm::vec3 &center);
  void setDistance(float distance);
  void setAngles(float azimuth, float elevation);
  void setZoomRange(float minDistance, float maxDistance);
  void setSensitivity(float rotationSensitivity, float zoomSensitivity);

  // Input handling
  void handleMouseInput(GLFWwindow *window, double deltaTime);
  void handleScrollInput(double yOffset);
  void setEnabled(bool enabled) { _enabled = enabled; }
  bool isEnabled() const { return _enabled; }

  // Getters
  float getDistance() const { return _distance; }
  float getAzimuth() const { return _azimuth; }
  float getElevation() const { return _elevation; }
  const glm::vec3 &getOrbitCenter() const { return _orbitCenter; }

  // Update camera position based on orbit parameters
  void updateOrbitPosition();

private:
  // Orbit parameters
  glm::vec3 _orbitCenter;
  float _distance;
  float _azimuth;   // Horizontal rotation (radians)
  float _elevation; // Vertical rotation (radians)

  // Constraints
  float _minDistance;
  float _maxDistance;
  float _minElevation;
  float _maxElevation;

  // Input sensitivity
  float _rotationSensitivity;
  float _zoomSensitivity;

  // Input state
  bool _enabled;
  bool _isMousePressed;
  double _lastMouseX;
  double _lastMouseY;

  // Helper methods
  void _clampAngles();
  void _clampDistance();
  glm::vec3 _calculatePosition() const;
};