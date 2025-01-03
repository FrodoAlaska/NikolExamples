#pragma once

#include "mesh.h"
#include "material.h"
#include "trasform.h"
#include "camera.h"

#include <nikol/nikol_core.hpp>

// ----------------------------------------------------------------------------
// Renderer 
struct Renderer;
// Renderer 
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Renderer functions
Renderer* renderer_create(nikol::Window* window);
void renderer_destroy(Renderer* renderer);

void renderer_clear(Renderer* renderer, const glm::vec4& color);
void renderer_begin(Renderer* renderer, const Camera& cam);
void renderer_end(Renderer* renderer);

nikol::GfxContext* renderer_get_gfx_context(Renderer* renderer);

void render_mesh(Renderer* renderer, Mesh* mesh, Material* material, Transform& transform);
// Renderer functions
// ----------------------------------------------------------------------------
