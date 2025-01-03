#pragma once

#include <nikol/nikol_core.hpp>

#include <glm/glm.hpp>

void renderer_create(nikol::Window* window);
void renderer_destroy();

void renderer_clear(const glm::vec4& color);
void renderer_begin();
void renderer_end();

void render_texture(nikol::GfxTexture* texture, const glm::vec2& pos, const glm::vec2& size, const glm::vec4& tint = glm::vec4(1.0f));
void render_quad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);

nikol::GfxTexture* renderer_load_texture(const char* path);
