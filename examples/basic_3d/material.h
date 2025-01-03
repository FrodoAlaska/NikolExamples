#pragma once

#include <nikol/nikol_core.hpp>
#include <glm/mat4x4.hpp>

// ----------------------------------------------------------------------------
// Material
struct Material {
  nikol::GfxContext* gfx         = nullptr;
  nikol::GfxShader* shader       = nullptr; 
  nikol::GfxTexture* diffuse     = nullptr;
  nikol::GfxBuffer* uniform_buff = nullptr;

  glm::mat4 transform;
};
// Material
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Material functions
Material* material_create(nikol::GfxContext* gfx, nikol::GfxShader* shader, nikol::GfxTexture* texture);
void material_destroy(Material* mat);
void material_apply(Material* mat);
// Material functions
// ----------------------------------------------------------------------------
