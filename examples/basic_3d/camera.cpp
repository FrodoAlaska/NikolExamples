#include "camera.h"

#include <nikol/nikol_core.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

// ----------------------------------------------------------------------------
// Globals
const float CAM_MAX_DEG  = 89.0f;
const float CAM_MAX_ZOOM = 180.0f;
const float CAM_SPEED    = 10.0f;
// Globals
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Public functions
Camera camera_create(const glm::vec3& position, const glm::vec3& target) {
  Camera  cam; 
 
  cam.yaw   = -90.0f;
  cam.pitch = 0.0f;
  cam.zoom  = 45.0f;
  cam.type  = CAMERA_FREE;

  glm::vec3 look_dir   = glm::vec3(position - target); 
  glm::vec3 up_axis    = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 right_axis = glm::normalize(glm::cross(up_axis, look_dir));
  
  cam.position    = position; 
  cam.up          = glm::normalize(glm::cross(look_dir, right_axis));
  cam.direction.x = glm::cos(glm::radians(cam.yaw)) * glm::cos(glm::radians(cam.pitch));
  cam.direction.y = glm::sin(glm::radians(cam.pitch));
  cam.direction.z = glm::sin(glm::radians(cam.yaw)) * glm::cos(glm::radians(cam.pitch));
  cam.front       = glm::normalize(cam.direction);
  
  cam.view = glm::mat4(1.0f);
  cam.projection = glm::mat4(1.0f);
  cam.view_projection = glm::mat4(1.0f);

  cam.can_move = true;

  return cam;
}

void camera_update(Camera& camera, nikol::Window* window) {
  float aspect_ratio = nikol::window_get_aspect_ratio(window);

  camera.view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
  camera.projection = glm::perspective(glm::radians(camera.zoom), aspect_ratio, 0.1f, 100.0f);
  camera.view_projection = camera.projection * camera.view;

  if(camera.can_move) {
    glm::vec2 mouse_offset;
    nikol::input_mouse_offset(&mouse_offset.x, &mouse_offset.y);  

    camera.yaw   = mouse_offset.x;
    camera.pitch = mouse_offset.y;
  }
  else {
    camera.yaw   = 0.0f;
    camera.pitch = 0.0f;
  }

  camera.pitch = glm::clamp(camera.pitch, -CAM_MAX_DEG, CAM_MAX_DEG);
  camera.zoom  = glm::clamp(camera.zoom, 1.0f, CAM_MAX_ZOOM);

  camera.direction.x = glm::cos(glm::radians(camera.yaw)) * glm::cos(glm::radians(camera.pitch));
  camera.direction.y = glm::sin(glm::radians(camera.pitch));
  camera.direction.z = glm::sin(glm::radians(camera.yaw)) * glm::cos(glm::radians(camera.pitch));
  camera.front       = glm::normalize(camera.direction);
}

void camera_move(Camera& camera) {
  float speed = CAM_SPEED * nikol::niclock_get_delta_time();

  // Move forward
  if(nikol::input_key_down(nikol::KEY_UP)) {
    camera.position += camera.front * speed;
  }
  // Move backwards
  else if(nikol::input_key_down(nikol::KEY_DOWN)) {
    camera.position -= camera.front * speed;
  }
 
  // Move right
  if(nikol::input_key_down(nikol::KEY_RIGHT)) {
    camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * speed;
  }
  // Move left
  else if(nikol::input_key_down(nikol::KEY_LEFT)) {
    camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * speed;
  }

  // Don't move on the Y-axis if the camera fixed 
  if(camera.type == CAMERA_FIXED) {
    camera.position.y = 0.0f;
  }
}
// Public functions
// ----------------------------------------------------------------------------
