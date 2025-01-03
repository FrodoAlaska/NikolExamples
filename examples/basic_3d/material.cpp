#include "material.h"

#include <nikol/nikol_core.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

// ----------------------------------------------------------------------------
// Material functions
Material* material_create(nikol::GfxContext* gfx, nikol::GfxShader* shader, nikol::GfxTexture* texture) {
  Material* mat = (Material*)nikol::memory_allocate(sizeof(Material));

  mat->gfx     = gfx;
  mat->diffuse = texture;
  mat->shader  = shader; 
  
  nikol::GfxBufferDesc buff_desc = {
    .data = nullptr,
    .size = sizeof(glm::mat4),
    .type = nikol::GFX_BUFFER_UNIFORM, 
    .usage = nikol::GFX_BUFFER_USAGE_DYNAMIC_DRAW,
  };
  mat->uniform_buff = nikol::gfx_buffer_create(gfx, buff_desc);

  nikol::gfx_shader_attach_uniform(gfx, shader, nikol::GFX_SHADER_VERTEX, mat->uniform_buff);

  return mat;
}

void material_destroy(Material* mat) {
  if(!mat) {
    return;
  }
 
  nikol::gfx_buffer_destroy(mat->uniform_buff);
  nikol::memory_free(mat);
}

void material_apply(Material* mat) {
  nikol::gfx_buffer_update(mat->gfx, mat->uniform_buff, 0, sizeof(glm::mat4), glm::value_ptr(mat->transform));
}
// Material functions
// ----------------------------------------------------------------------------
