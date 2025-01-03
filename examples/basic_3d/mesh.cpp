#include "mesh.h"
#include "vertex.h"

#include <nikol/nikol_core.hpp>
#include <glm/glm.hpp>

#include <vector>

// ----------------------------------------------------------------------------
// Private functions
static void construct_cube_mesh(std::vector<Vertex>& vertices, std::vector<nikol::u32>& indices) {
  // Position - Normal - Texture coords
  // Back-face
  vertices.push_back({glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});  
  vertices.push_back({glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)}); 
  vertices.push_back({glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)}); 
  vertices.push_back({glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)}); 

  // Front-face
  vertices.push_back({glm::vec3(-0.5f, -0.5f, 0.5f),  glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec2(0.0f, 0.0f)});
  vertices.push_back({glm::vec3( 0.5f, -0.5f, 0.5f),  glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec2(1.0f, 0.0f)});
  vertices.push_back({glm::vec3( 0.5f,  0.5f, 0.5f),  glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec2(1.0f, 1.0f)});
  vertices.push_back({glm::vec3(-0.5f,  0.5f, 0.5f),  glm::vec3(0.0f, 0.0f, 1.0f),  glm::vec2(0.0f, 1.0f)});

  // Left-face
  vertices.push_back({glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
  vertices.push_back({glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)});
  vertices.push_back({glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
  vertices.push_back({glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});

  // Right-face
  vertices.push_back({glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec2(1.0f, 0.0f)});
  vertices.push_back({glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec2(1.0f, 1.0f)});
  vertices.push_back({glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec2(0.0f, 1.0f)});
  vertices.push_back({glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec2(0.0f, 0.0f)});

  // Top-face
  vertices.push_back({glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
  vertices.push_back({glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)});
  vertices.push_back({glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
  vertices.push_back({glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});

  // Bottom-face
  vertices.push_back({glm::vec3(-0.5f, 0.5f, -0.5f),  glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec2(0.0f, 1.0f)});
  vertices.push_back({glm::vec3( 0.5f, 0.5f, -0.5f),  glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec2(1.0f, 1.0f)});
  vertices.push_back({glm::vec3( 0.5f, 0.5f,  0.5f),  glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec2(1.0f, 0.0f)});
  vertices.push_back({glm::vec3(-0.5f, 0.5f,  0.5f),  glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec2(0.0f, 0.0f)});
  
  indices = {
    // Back-face 
    0, 1, 2, 
    2, 3, 0, 

    // Front-face 
    4, 5, 6, 
    6, 7, 4, 

    // Left-face 
    10, 9, 8, 
    8, 11, 10, 
    
    // Right-face 
    14, 13, 12, 
    12, 15, 14,

    // Top-face 
    16, 17, 18, 
    18, 19, 16, 

    // Bottom-face 
    20, 21, 22, 
    22, 23, 20, 
  };
}

static void construct_sphere_mesh(std::vector<Vertex>& vertices, std::vector<nikol::u32>& indices) {
  // @TODO
}

static void construct_mesh_by_type(const MeshType type, std::vector<Vertex>& vertices, std::vector<nikol::u32>& indices) {
  switch(type) {
    case MESH_TYPE_CUBE:
      construct_cube_mesh(vertices, indices);
      break;
    case MESH_TYPE_SPHERE:
      construct_sphere_mesh(vertices, indices);
      break;
  }
}

// Private functions
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Mesh functions
Mesh* mesh_create(nikol::GfxContext* gfx, const std::vector<Vertex>& vertices, const std::vector<nikol::u32>& indices) {
  Mesh* mesh = (Mesh*)nikol::memory_allocate(sizeof(Mesh));
  
  // Vertex buffer init
  nikol::GfxBufferDesc vert_desc = {
    .data  = (void*)vertices.data(),
    .size  = vertices.size() * sizeof(Vertex),
    .type  = nikol::GFX_BUFFER_VERTEX, 
    .usage = nikol::GFX_BUFFER_USAGE_STATIC_DRAW,
  };
  mesh->pipe_desc.vertex_buffer  = nikol::gfx_buffer_create(gfx, vert_desc);
  mesh->pipe_desc.vertices_count = vertices.size();  

  // Index buffer init
  nikol::GfxBufferDesc index_desc = {
    .data  = (void*)indices.data(),
    .size  = indices.size() * sizeof(nikol::u32),
    .type  = nikol::GFX_BUFFER_INDEX, 
    .usage = nikol::GFX_BUFFER_USAGE_STATIC_DRAW,
  };
  mesh->pipe_desc.index_buffer  = nikol::gfx_buffer_create(gfx, index_desc);
  mesh->pipe_desc.indices_count = indices.size();  

  // Shader init (will later be filled with the appropriate material)
  mesh->pipe_desc.shader = nullptr; 

  // Layout init
  mesh->pipe_desc.layout[0]    = nikol::GfxLayoutDesc{"POS", nikol::GFX_LAYOUT_FLOAT3, 0};
  mesh->pipe_desc.layout[1]    = nikol::GfxLayoutDesc{"NORMAL", nikol::GFX_LAYOUT_FLOAT3, 0};
  mesh->pipe_desc.layout[2]    = nikol::GfxLayoutDesc{"TEXCOORDS0", nikol::GFX_LAYOUT_FLOAT2, 0};
  mesh->pipe_desc.layout_count = 3;

  // Draw mode init 
  mesh->pipe_desc.draw_mode = nikol::GFX_DRAW_MODE_TRIANGLE;

  // Finally, creating the pipeline 
  mesh->pipe = nikol::gfx_pipeline_create(gfx, mesh->pipe_desc);

  return mesh;
}

Mesh* mesh_create(nikol::GfxContext* gfx, const MeshType type) {
  std::vector<Vertex> vertices;
  std::vector<nikol::u32> indices;
  construct_mesh_by_type(type, vertices, indices);

  return mesh_create(gfx, vertices, indices);
}

void mesh_destroy(Mesh* mesh) {
  if(!mesh) {
    return;
  }
 
  nikol::gfx_pipeline_destroy(mesh->pipe);
  nikol::memory_free(mesh);
}
// Mesh functions
// ----------------------------------------------------------------------------
