#pragma once

#include <nikol/nikol_core.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------
// CameraType
enum CameraType {
  CAMERA_FREE, 
  CAMERA_FIXED,
};
// CameraType
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Camera
struct Camera {
  nikol::f32 yaw, pitch, zoom;
  CameraType type;

  glm::vec3 position, up, direction, front;
  glm::mat4 view, projection, view_projection;

  bool can_move;
};
// Camera
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Public functions
Camera camera_create(const glm::vec3& position, const glm::vec3& target);
void camera_update(Camera& camera, nikol::Window* window);
void camera_move(Camera& camera);
// Public functions
// ----------------------------------------------------------------------------
