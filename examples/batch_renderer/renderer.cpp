#include "renderer.h"

#include <vector>

#include <stb/stb_image.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

/// --------------------------------------------------------------------------------------
/// DEFS
#define MAX_QUADS     10000
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
  float texture_index;
};
/// Vertex 
/// --------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------
/// Renderer
struct Renderer {
  nikol::GfxContextDesc gfx_desc;
  nikol::GfxContext* gfx = nullptr;

  nikol::GfxBufferDesc buff_desc;
  nikol::GfxBuffer* vertex_buff = nullptr; 

  nikol::GfxPipelineDesc pipe_desc;
  nikol::GfxPipeline* pipe = nullptr;

  std::vector<Vertex> vertices;

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
  return 
    "#version 460 core\n"
    "\n"
    "// Layouts\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTextureCoords;\n"
    "layout (location = 2) in vec4 aColor;\n"
    "layout (location = 3) in float aTextureIndex;\n"
    "\n"
    "// Outputs\n"
    "out VS_OUT {\n"
    "  vec4 out_color;\n"
    "  vec2 tex_coords;\n"
    "  float tex_index;\n"
    "} vs_out;\n"
    "\n"
    "void main() {\n"
    "  vs_out.out_color  = aColor;\n"
    "  vs_out.tex_coords = aTextureCoords;\n"
    "  vs_out.tex_index  = aTextureIndex;\n"
    "\n"
    "  gl_Position = vec4(aPos, 1.0f);\n"
    "}\n"
    "\n"
    "#version 460 core\n"
    "\n"
    "// Outputs\n"
    "layout (location = 0) out vec4 frag_color;\n"
    "\n"
    "// Inputs\n"
    "in VS_OUT {\n"
    "  vec4 out_color;\n"
    "  vec2 tex_coords;\n"
    "  float tex_index;\n"
    "} fs_in;\n"
    "\n"
    "// Uniforms\n"
    "uniform sampler2D u_textures[32];\n"
    "\n"
    "void main() {\n"
    "  int index  = int(fs_in.tex_index);\n"
    "  frag_color = texture(u_textures[index], fs_in.tex_coords) * fs_in.out_color;\n"
    "}\n";
#elif defined(NIKOL_GFX_CONTEXT_DX11)
  return 
    "struct vs_in {\n"
    "    float3 position   : POS;\n"
    "    float2 tex_coords : TEX;\n"
    "    float4 color      : COLOR;\n"
    "    float1 tex_index  : INDEX;\n"
    "};\n"
    "\n"
    "struct vs_out {\n"
    "    float4 position   : SV_POSITION;\n"
    "    float2 tex_coords : TEX;\n"
    "    float4 color      : COLOR;\n"
    "    float1 tex_index  : INDEX;\n"
    "};\n"
    "\n"
    "Texture2DArray textures    : register(t0);\n"
    "SamplerState samp          : register(s0);\n"
    "\n"
    "vs_out vs_main(vs_in input) {\n"
    "    vs_out output = (vs_out)0;\n"
    "\n"
    "    output.position   = float4(input.position, 1.0);\n"
    "    output.tex_coords = input.tex_coords;\n"
    "    output.color      = input.color;\n"
    "    output.tex_index  = input.tex_index;\n"
    "\n" 
    "    return output;\n"
    "}\n"
    "\n"
    "float4 ps_main(vs_out input) : SV_TARGET {\n"
    "  float4 color;\n"
    "  color = textures.Sample(samp, float3(input.tex_coords.x, input.tex_coords.y, input.tex_index)) * input.color;\n"
    "  return color;\n"
    "}\n";
#endif
}

static void init_pipeline() {
  // Vertex buffer init
  s_renderer.buff_desc = {
    .data  = nullptr, 
    .size  = sizeof(Vertex) * MAX_VERTICES, 
    .type  = nikol::GFX_BUFFER_VERTEX, 
    .usage = nikol::GFX_BUFFER_USAGE_DYNAMIC_DRAW,
  };
  s_renderer.pipe_desc.vertex_buffer = nikol::gfx_buffer_create(s_renderer.gfx, s_renderer.buff_desc);

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
  s_renderer.pipe_desc.layout[3] = nikol::GfxLayoutDesc{"INDEX", nikol::GFX_LAYOUT_FLOAT1, 0};
  s_renderer.pipe_desc.layout_count = 4;

  // Draw mode init 
  s_renderer.pipe_desc.draw_mode = nikol::GFX_DRAW_MODE_TRIANGLE;

  // Textures init
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
  s_renderer.pipe_desc.textures[0]    = nikol::gfx_texture_create(s_renderer.gfx, texture_desc);
  s_renderer.pipe_desc.textures_count = 1;

  // Pipeline init
  s_renderer.pipe = nikol::gfx_pipeline_create(s_renderer.gfx, s_renderer.pipe_desc);
}

static void flush_renderer() {
  // Update the vertex buffer 
  nikol::gfx_buffer_update(s_renderer.gfx, 
                           s_renderer.pipe_desc.vertex_buffer, 
                           0, 
                           sizeof(Vertex) * s_renderer.vertices.size(), 
                           s_renderer.vertices.data());

  // Apply the pipeline 
  nikol::gfx_context_apply_pipeline(s_renderer.gfx, s_renderer.pipe, s_renderer.pipe_desc);

  // Draw the pipeline
  nikol::gfx_pipeline_draw_index(s_renderer.gfx, s_renderer.pipe);
  
  // Reset the indices and the textures
  s_renderer.pipe_desc.indices_count  = 0;
  s_renderer.pipe_desc.textures_count = 1;
}

static int find_texture(const nikol::GfxTexture* texture) {
  int index = -1;

  for(int i = 0; i < s_renderer.pipe_desc.textures_count; i++) {
    nikol::GfxTexture* pipe_tex = s_renderer.pipe_desc.textures[i];

    if(pipe_tex != texture) {
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
  s_renderer.vertices.clear();
}

void renderer_end() {
  flush_renderer();

  nikol::gfx_context_present(s_renderer.gfx);
}

void render_texture(nikol::GfxTexture* texture, const glm::vec2& pos, const glm::vec2& size, const glm::vec4& tint) {
  bool can_flush = s_renderer.pipe_desc.indices_count >= MAX_INDICES || s_renderer.pipe_desc.textures_count >= nikol::TEXTURES_MAX;
  if(can_flush) {
    flush_renderer();
  }

  int index = find_texture(texture);
  
  // Adding the texture to be drawn later if it's new
  if(index == -1) {
    s_renderer.pipe_desc.textures[s_renderer.pipe_desc.textures_count] = texture;
    index = s_renderer.pipe_desc.textures_count++;
  }

  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) * 
                    glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 0.0f));
  glm::mat4 world_pos =  s_renderer.ortho_cam * model;

  // Top-left 
  Vertex v1; 
  v1.pos            = world_pos * s_renderer.quad_vertices[0]; 
  v1.color          = tint;
  v1.texture_coords = pos / size; 
  v1.texture_index  = index;
  s_renderer.vertices.push_back(v1);
 
  // Top-right
  Vertex v2; 
  v2.pos            = world_pos * s_renderer.quad_vertices[1]; 
  v2.color          = tint;
  v2.texture_coords = glm::vec2(pos.x + size.x, pos.y) / size; 
  v2.texture_index  = index;
  s_renderer.vertices.push_back(v2);
 
  // Bottom-right
  Vertex v3; 
  v3.pos            = world_pos * s_renderer.quad_vertices[2]; 
  v3.color          = tint;
  v3.texture_coords = (pos + size) / size; 
  v3.texture_index  = index;
  s_renderer.vertices.push_back(v3);
 
  // Bottom-left
  Vertex v4; 
  v4.pos            = world_pos * s_renderer.quad_vertices[3]; 
  v4.color          = tint;
  v4.texture_coords = glm::vec2(pos.x, pos.y + size.y) / size; 
  v4.texture_index  = index;
  s_renderer.vertices.push_back(v4);

  s_renderer.pipe_desc.indices_count  += 6;
}

void render_quad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
  if(s_renderer.pipe_desc.indices_count >= MAX_INDICES) {
    flush_renderer();
  }

  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) * 
                    glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 0.0f));
  glm::mat4 world_pos =  s_renderer.ortho_cam * model;

  // Top-left 
  Vertex v1; 
  v1.pos            = world_pos * s_renderer.quad_vertices[0]; 
  v1.color          = color;
  v1.texture_coords = glm::vec2(0.0f, 0.0f); 
  v1.texture_index  = 0.0f;
  s_renderer.vertices.push_back(v1);

  // Top-right
  Vertex v2; 
  v2.pos            = world_pos * s_renderer.quad_vertices[1]; 
  v2.color          = color;
  v2.texture_coords = glm::vec2(1.0f, 0.0f); 
  v2.texture_index  = 0.0f;
  s_renderer.vertices.push_back(v2);

  // Bottom-right
  Vertex v3; 
  v3.pos            = world_pos * s_renderer.quad_vertices[2]; 
  v3.color          = color;
  v3.texture_coords = glm::vec2(1.0f, 1.0f); 
  v3.texture_index  = 0.0f;
  s_renderer.vertices.push_back(v3);

  // Bottom-left
  Vertex v4; 
  v4.pos            = world_pos * s_renderer.quad_vertices[3]; 
  v4.color          = color;
  v4.texture_coords = glm::vec2(0.0f, 1.0f); 
  v4.texture_index  = 0.0f;
  s_renderer.vertices.push_back(v4);

  s_renderer.pipe_desc.indices_count += 6;
}

nikol::GfxTexture* renderer_load_texture(const char* path) {
  nikol::GfxTextureDesc desc; 

  int width = 1, height = 1, channels;

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

  return nikol::gfx_texture_create(s_renderer.gfx, desc);
}

/// Public functions
/// --------------------------------------------------------------------------------------
