#version 460 core

// Layouts
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoords;

// Outputs
out VS_OUT {
  vec2 tex_coords;
} vs_out;

layout (std140, binding = 0) uniform Matrices {
  mat4 view_projection;
};

void main() {
  vs_out.tex_coords = aTextureCoords;

  gl_Position = view_projection * vec4(aPos, 1.0f);
}

#version 460 core

// Outputs
layout (location = 0) out vec4 frag_color;

// Inputs\n"
in VS_OUT {
  vec2 tex_coords;
} fs_in;

// Uniforms
uniform sampler2D u_texture;

void main() {
  frag_color = texture(u_texture, fs_in.tex_coords);
};
