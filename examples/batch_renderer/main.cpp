#include "renderer.h"

const float SIZE = 64.0f;
const int WINDOW_WIDTH = 1280; 
const int WINDOW_HEIGHT = 640; 


static nikol::GfxTexture* get_platform_logo_texture() {
#if defined(NIKOL_GFX_CONTEXT_OPENGL) 
  return renderer_load_texture("assets/opengl.png"); 
#else
  return renderer_load_texture("assets/dx11.png"); 
#endif
}

int main() {
  if(!nikol::init()) {
    return -1;
  } 

  nikol::u32 win_flags = nikol::WINDOW_FLAGS_FOCUS_ON_SHOW | nikol::WINDOW_FLAGS_GFX_HARDWARE | nikol::WINDOW_FLAGS_RESIZABLE;
  nikol::Window* window = nikol::window_open("Batch Renderer", WINDOW_WIDTH, WINDOW_HEIGHT, win_flags);
  NIKOL_ASSERT(window, "Could not open a window");

  renderer_create(window);

  int total_x = 30;
  int total_y = 20;

  nikol::GfxTexture* texture = get_platform_logo_texture();

  while(nikol::window_is_open(window)) {
    if(nikol::input_key_pressed(nikol::KEY_ESCAPE)) {
      break;
    }
  
    float x = 1.0f + glm::sin(nikol::niclock_get_time()) * 2.0f; 
    float y = glm::sin(nikol::niclock_get_time() / 2.0f) * 1.0f;

    renderer_clear(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    renderer_begin();

    for(int i = 1; i <= total_y; i++) {
      for(int j = 1; j <= total_x; j++) {
        render_texture(texture, glm::vec2((j - 1) * (SIZE + 1), (i - 1) * (SIZE + 1)), glm::vec2(SIZE), glm::vec4((x + y) * (j + i), y, x, 1.0f));
      }
    }

    renderer_end();
    nikol::window_poll_events(window);
  }

  renderer_destroy();
  nikol::window_close(window);
  nikol::shutdown();
}
