#include "renderer.h"
#include "nikol/nikol_core.hpp"
#include "shaders.h"

#include <vector>

#include <stb/stb_image.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

/// --------------------------------------------------------------------------------------
/// DEFS
#define MAX_QUADS     1000
#define MAX_VERTICES  MAX_QUADS * 4
#define MAX_INDICES   MAX_QUADS * 6
/// DEFS
/// --------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------
/// Vertex 
struct Vertex {
  glm::vec3 pos; 
  glm::vec2 texture_coords;
  glm::vec4 color;
};
/// Vertex 
/// --------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------
/// BatchDrawCall 
struct BatchDrawCall {
  nikol::sizei indices_count = 0; 
  nikol::GfxTexture* texture = nullptr;
  std::vector<Vertex> vertices;
};
/// BatchDrawCall 
/// --------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------
/// Renderer
struct Renderer {
  nikol::GfxContextDesc gfx_desc;
  nikol::GfxContext* gfx = nullptr;

  nikol::GfxPipelineDesc pipe_desc;
  nikol::GfxPipeline* pipe = nullptr;

  std::vector<BatchDrawCall> batches;

  glm::vec4 quad_vertices[4]; 
  glm::mat4 ortho_cam;
};

static Renderer s_renderer;
/// Renderer
/// --------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------
/// Private functions

static void init_context(nikol::Window* window) {
  s_renderer.gfx_desc = {
    .window = window,
    .states = nikol::GFX_STATE_DEPTH | nikol::GFX_STATE_STENCIL | nikol::GFX_STATE_BLEND,
  };

  s_renderer.gfx = nikol::gfx_context_init(s_renderer.gfx_desc);
  NIKOL_ASSERT(s_renderer.gfx, "Could not initialize a graphics context");
}

static const char* compile_shaders() {
#if defined(NIKOL_GFX_CONTEXT_OPENGL)
  return batch_glsl_code();
#elif defined(NIKOL_GFX_CONTEXT_DX11)
  return batch_hlsl_code();
#endif
}

static void init_pipeline() {
  // Vertex buffer init
  nikol::GfxBufferDesc buff_desc = {
    .data  = nullptr, 
    .size  = sizeof(Vertex) * MAX_VERTICES, 
    .type  = nikol::GFX_BUFFER_VERTEX, 
    .usage = nikol::GFX_BUFFER_USAGE_DYNAMIC_DRAW,
  };
  s_renderer.pipe_desc.vertex_buffer = nikol::gfx_buffer_create(s_renderer.gfx, buff_desc);

  // Index buffer init
  nikol::u32 indices[MAX_INDICES];
  nikol::u32 offset = 0;
  for(int i = 0; i < MAX_INDICES; i += 6) {
    indices[i + 0] = 0 + offset;
    indices[i + 1] = 1 + offset;
    indices[i + 2] = 2 + offset;
    
    indices[i + 3] = 2 + offset;
    indices[i + 4] = 3 + offset;
    indices[i + 5] = 0 + offset;

    offset += 4; 
  }
  nikol::GfxBufferDesc index_buff_desc = {
    .data  = indices, 
    .size  = sizeof(indices), 
    .type  = nikol::GFX_BUFFER_INDEX, 
    .usage = nikol::GFX_BUFFER_USAGE_STATIC_DRAW,
  };
  s_renderer.pipe_desc.index_buffer  = nikol::gfx_buffer_create(s_renderer.gfx, index_buff_desc);

  // Shader init   
  s_renderer.pipe_desc.shader = nikol::gfx_shader_create(s_renderer.gfx, compile_shaders());
  
  // Layout init
  s_renderer.pipe_desc.layout[0] = nikol::GfxLayoutDesc{"POS", nikol::GFX_LAYOUT_FLOAT3, 0};
  s_renderer.pipe_desc.layout[1] = nikol::GfxLayoutDesc{"TEX", nikol::GFX_LAYOUT_FLOAT2, 0};
  s_renderer.pipe_desc.layout[2] = nikol::GfxLayoutDesc{"COLOR", nikol::GFX_LAYOUT_FLOAT4, 0};
  s_renderer.pipe_desc.layout_count = 3;

  // Draw mode init 
  s_renderer.pipe_desc.draw_mode = nikol::GFX_DRAW_MODE_TRIANGLE;

  // s_renderer.pipe_desc.textures[0]    = nikol::gfx_texture_create(s_renderer.gfx, texture_desc);

  // Pipeline init
  s_renderer.pipe = nikol::gfx_pipeline_create(s_renderer.gfx, s_renderer.pipe_desc);
}

static void flush_batch(BatchDrawCall& draw_call) {
  // An empty vertices array is useless... 
  if(draw_call.vertices.size() == 0) {
    return;
  }

  // Set the pipeline according to the draw call
  s_renderer.pipe_desc.indices_count    = draw_call.indices_count;
  s_renderer.pipe_desc.textures[0]      = draw_call.texture;
  s_renderer.pipe_desc.textures_count   = 1;

  // Update the vertex buffer 
  nikol::gfx_buffer_update(s_renderer.gfx, 
                           s_renderer.pipe_desc.vertex_buffer, 
                           0, 
                           sizeof(Vertex) * draw_call.vertices.size(), 
                           draw_call.vertices.data());

  // Apply the pipeline 
  nikol::gfx_context_apply_pipeline(s_renderer.gfx, s_renderer.pipe, s_renderer.pipe_desc);

  // Draw the pipeline
  nikol::gfx_pipeline_draw_index(s_renderer.gfx, s_renderer.pipe);
  
  // Reset back to normal
  draw_call.indices_count  = 0;
  draw_call.vertices.clear();
}

static int find_texture(const nikol::GfxTexture* texture) {
  int index = -1;

  for(int i = 0; i < s_renderer.batches.size(); i++) {
    nikol::GfxTexture* batch_texture = s_renderer.batches[i].texture;

    if(batch_texture != texture) {
      continue;
    }
      
    index = i;
    break;
  }

  return index;
}

/// Private functions
/// --------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------
/// Public functions

void renderer_create(nikol::Window* window) {
  init_context(window);
  init_pipeline();

  // Allocate some memory for the batches
  s_renderer.batches.reserve(32);

  // Create a white texture batch  
  nikol::u32 pixels = 0xffffffff;
  nikol::GfxTextureDesc texture_desc = {
    .width     = 1, 
    .height    = 1, 
    .depth     = 0, 
    .format    = nikol::GFX_TEXTURE_FORMAT_RGBA8, 
    .filter    = nikol::GFX_TEXTURE_FILTER_MIN_MAG_NEAREST, 
    .wrap_mode = nikol::GFX_TEXTURE_WRAP_REPEAT, 
    .data      = &pixels,
  };
  BatchDrawCall draw_call = {
    .indices_count = 0, 
    .texture       = nikol::gfx_texture_create(s_renderer.gfx, texture_desc),
  };
  s_renderer.batches.push_back(draw_call); 
  
  // Quad vertices init
  s_renderer.quad_vertices[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
  s_renderer.quad_vertices[1] = glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f);
  s_renderer.quad_vertices[2] = glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f);
  s_renderer.quad_vertices[3] = glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f);
}

void renderer_destroy() {
  nikol::gfx_pipeline_destroy(s_renderer.pipe);
  nikol::gfx_context_shutdown(s_renderer.gfx);
}

void renderer_clear(const glm::vec4& color) {
  nikol::gfx_context_clear(s_renderer.gfx, color.r, color.g, color.b, color.a);
}

void renderer_begin() {
  int width, height;
  nikol::window_get_size(s_renderer.gfx_desc.window, &width, &height);

  s_renderer.ortho_cam = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
}

void renderer_end() {
  for(auto& batch : s_renderer.batches) {
    flush_batch(batch);
  }

  nikol::gfx_context_present(s_renderer.gfx);
}

void render_texture(nikol::GfxTexture* texture, const glm::vec2& pos, const glm::vec2& size, const glm::vec4& tint) {
  int index                = find_texture(texture);
  BatchDrawCall* draw_call = &s_renderer.batches[index];

  if(draw_call->indices_count >= MAX_INDICES) {
    flush_batch(*draw_call);
  }

  glm::mat4 model     = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) * 
                        glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 0.0f));
  glm::mat4 world_pos =  s_renderer.ortho_cam * model;

  // Top-left 
  Vertex v1 = {
    .pos            = world_pos * s_renderer.quad_vertices[0], 
    .texture_coords = pos / size, 
    .color          = tint,
  };
  draw_call->vertices.push_back(v1);

  // Top-right
  Vertex v2 = {
    .pos            = world_pos * s_renderer.quad_vertices[1], 
    .texture_coords = glm::vec2(pos.x + size.x, pos.y) / size, 
    .color          = tint,
  };
  draw_call->vertices.push_back(v2);

  // Bottom-right
  Vertex v3 = {
    .pos            = world_pos * s_renderer.quad_vertices[2], 
    .texture_coords = (pos + size) / size, 
    .color          = tint,
  }; 
  draw_call->vertices.push_back(v3);

  // Bottom-left
  Vertex v4 = {
    .pos            = world_pos * s_renderer.quad_vertices[3], 
    .texture_coords = glm::vec2(pos.x, pos.y + size.y) / size, 
    .color          = tint,
  }; 
  draw_call->vertices.push_back(v4);

  draw_call->indices_count  += 6;
}

void render_quad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
  BatchDrawCall* draw_call = &s_renderer.batches[0];

  if(draw_call->indices_count >= MAX_INDICES) {
    flush_batch(*draw_call);
  }

  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) * 
                    glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 0.0f));
  glm::mat4 world_pos =  s_renderer.ortho_cam * model;

  // Top-left 
  Vertex v1 = {
    .pos            = world_pos * s_renderer.quad_vertices[0],
    .texture_coords = glm::vec2(0.0f, 0.0f),
    .color          = color,
  }; 
  draw_call->vertices.push_back(v1);

  // Top-right
  Vertex v2 = {
    .pos            = world_pos * s_renderer.quad_vertices[1], 
    .texture_coords = glm::vec2(1.0f, 0.0f), 
    .color          = color,
  };
  draw_call->vertices.push_back(v2);

  // Bottom-right
  Vertex v3 = {
    .pos            = world_pos * s_renderer.quad_vertices[2], 
    .texture_coords = glm::vec2(1.0f, 1.0f), 
    .color          = color,
  };
  draw_call->vertices.push_back(v3);

  // Bottom-left
  Vertex v4 = {
    .pos            = world_pos * s_renderer.quad_vertices[3], 
    .texture_coords = glm::vec2(0.0f, 1.0f), 
    .color          = color,
  };
  draw_call->vertices.push_back(v4);

  draw_call->indices_count += 6;
}

nikol::GfxTexture* renderer_load_texture(const char* path) {
  nikol::GfxTextureDesc desc = {}; 
  nikol::GfxTexture* texture = nullptr;

  int width, height, channels;

  stbi_set_flip_vertically_on_load(false);
  nikol::u8* pixels = stbi_load(path, &width, &height, &channels, 4);
  NIKOL_ASSERT(pixels, "Could not load a texture");

  desc.width     = width; 
  desc.height    = height;
  desc.depth     = 0; 
  desc.format    = nikol::GFX_TEXTURE_FORMAT_RGBA8; 
  desc.filter    = nikol::GFX_TEXTURE_FILTER_MIN_MAG_NEAREST;
  desc.wrap_mode = nikol::GFX_TEXTURE_WRAP_REPEAT;
  desc.data      = pixels;
  
  texture = nikol::gfx_texture_create(s_renderer.gfx, desc);
  
  // Create a new batch for the texture 
  BatchDrawCall draw_call = {
    .indices_count = 0, 
    .texture       = texture,
  };
  s_renderer.batches.push_back(draw_call);

  return texture;
}

/// Public functions
/// --------------------------------------------------------------------------------------
