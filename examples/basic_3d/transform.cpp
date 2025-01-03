#include "trasform.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

// ----------------------------------------------------------------------------
// Private functions

static void update_transform(Transform& trans) {
  trans.model = glm::translate(glm::mat4(1.0f), trans.position) * 
                glm::rotate(glm::mat4(1.0f), 90.0f, trans.rotation) *
                glm::scale(glm::mat4(1.0f), trans.scale);
}

// Private functions
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Transform functions
Transform transform_create(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotation) {
  return Transform {
    .position = pos, 
    .rotation = rotation, 
    .scale    = scale, 
    .model    = glm::mat4(1.0f),
  };
}

void transform_translate(Transform& trans, const glm::vec3& pos) {
  trans.position = pos;
  update_transform(trans);  
}

void transform_rotate(Transform& trans, const float angle, const glm::vec3& axis) {
  trans.rotation = axis;
  update_transform(trans);  
}

void transform_scale(Transform& trans, const glm::vec3& scale) {
  trans.scale = scale;
  update_transform(trans);  
}
// Transform functions
// ----------------------------------------------------------------------------
