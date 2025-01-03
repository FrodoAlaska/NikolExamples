#pragma once

#include "vertex.h"

#include <nikol/nikol_core.hpp>

#include <vector>

// ----------------------------------------------------------------------------
// MeshType
enum MeshType {
  MESH_TYPE_CUBE = 0, 
  MESH_TYPE_SPHERE, 
};
// MeshType
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Mesh
struct Mesh {
  nikol::GfxPipelineDesc pipe_desc;
  nikol::GfxPipeline* pipe;
};
// Mesh
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Mesh functions
Mesh* mesh_create(nikol::GfxContext* gfx, const std::vector<Vertex>& vertices, const std::vector<nikol::u32>& indices);
Mesh* mesh_create(nikol::GfxContext* gfx, const MeshType type);
void mesh_destroy(Mesh* mesh);
// Mesh functions
// ----------------------------------------------------------------------------
