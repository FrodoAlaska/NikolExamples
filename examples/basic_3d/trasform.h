#pragma once

#include <glm/glm.hpp>

// ----------------------------------------------------------------------------
// Transform
struct Transform {
  glm::vec3 position; 
  glm::vec3 rotation; 
  glm::vec3 scale; 

  glm::mat4 model;
};
// Transform
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Transform functions
Transform transform_create(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotation = glm::vec3(0.0f));
void transform_translate(Transform& trans, const glm::vec3& pos);
void transform_rotate(Transform& trans, const float angle, const glm::vec3& axis);
void transform_scale(Transform& trans, const glm::vec3& scale);
// Transform functions
// ----------------------------------------------------------------------------
