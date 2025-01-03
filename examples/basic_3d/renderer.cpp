#include "renderer.h"
#include "mesh.h"
#include "material.h"
#include "trasform.h"
#include "camera.h"
#include "shaders.h"

#include <nikol/nikol_core.hpp>

#include <vector>

// ----------------------------------------------------------------------------
// Vertex3D 
struct Vertex3D {
  glm::vec3 position; 
  glm::vec3 normal;
  glm::vec2 texture_coords;
};
// Vertex3D 
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Renderer 
struct Renderer {
  nikol::GfxContext* gfx = nullptr; 

  nikol::GfxShader* default_shader = nullptr;
  nikol::GfxTexture* white_texture = nullptr;
  Material* default_material       = nullptr;

  std::vector<Mesh*> draw_calls;

  glm::mat4 view_proj;
};
// Renderer 
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Private functions
static const char* get_default_shader() {
#ifdef NIKOL_GFX_CONTEXT_OPENGL
  return default_shader_glsl();
#else 
  return default_shader_hlsl();
#endif
}
// Private functions
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Renderer functions
Renderer* renderer_create(nikol::Window* window) {
  Renderer* renderer = (Renderer*)nikol::memory_allocate(sizeof(Renderer)); 

  // Creating a graphics context
  nikol::GfxContextDesc gfx_desc = {
    .window = window,
    .states = nikol::GFX_STATE_DEPTH | nikol::GFX_STATE_STENCIL,
  };
  renderer->gfx = nikol::gfx_context_init(gfx_desc);
  NIKOL_ASSERT(renderer->gfx, "Could not create a graphics context");

  // Creating a default shader 
  renderer->default_shader = nikol::gfx_shader_create(renderer->gfx, get_default_shader());

  // Creating a white texture 
  nikol::u32 pixels = 0xffffffff; 
  nikol::GfxTextureDesc tex_desc = {
    .width  = 1, 
    .height = 1, 
    .depth  = 0, 
    .format = nikol::GFX_TEXTURE_FORMAT_RGBA8, 
    .filter = nikol::GFX_TEXTURE_FILTER_MIN_MAG_NEAREST, 
    .wrap_mode = nikol::GFX_TEXTURE_WRAP_REPEAT, 
    .data = &pixels,
  };
  renderer->white_texture = nikol::gfx_texture_create(renderer->gfx, tex_desc);

  // Creating a default material
  renderer->default_material = material_create(renderer->gfx, renderer->default_shader, renderer->white_texture);

  // Give some initial space 
  renderer->draw_calls.reserve(32);

  return renderer;
}

void renderer_destroy(Renderer* renderer) {
  if(!renderer) {
    return;
  }
  
  renderer->draw_calls.clear();
  material_destroy(renderer->default_material);
 
  nikol::gfx_texture_destroy(renderer->white_texture);
  nikol::gfx_shader_destroy(renderer->default_shader);
  
  nikol::gfx_context_shutdown(renderer->gfx);
  nikol::memory_free(renderer);
}

void renderer_clear(Renderer* renderer, const glm::vec4& color) {
  nikol::gfx_context_clear(renderer->gfx, color.r, color.g, color.b, color.a);
}

void renderer_begin(Renderer* renderer, const Camera& cam) {
  renderer->view_proj = cam.view_projection; 
  renderer->draw_calls.clear();
}

void renderer_end(Renderer* renderer) {
  for(auto& draw : renderer->draw_calls) {
    nikol::gfx_context_apply_pipeline(renderer->gfx, draw->pipe, draw->pipe_desc);
    nikol::gfx_pipeline_draw_index(renderer->gfx, draw->pipe);
  }
}

nikol::GfxContext* renderer_get_gfx_context(Renderer* renderer) {
  return renderer->gfx;
}

void render_mesh(Renderer* renderer, Mesh* mesh, Material* material, Transform& transform) {
  if(!material) {
    material = renderer->default_material;
  }

  mesh->pipe_desc.shader = material->shader;

  mesh->pipe_desc.textures[0]    = material->diffuse;
  mesh->pipe_desc.textures_count = 1; 

  material->transform = renderer->view_proj * transform.model;  
  material_apply(material);

  renderer->draw_calls.push_back(mesh);
}
// Renderer functions
// ----------------------------------------------------------------------------
