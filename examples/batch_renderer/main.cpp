#include "renderer.h"

#include <nikol/nikol_core.hpp>
#include <glm/glm.hpp>

int main() {
  if(!nikol::init()) {
    return -1;
  } 

  nikol::u32 win_flags = nikol::WINDOW_FLAGS_FOCUS_ON_SHOW | nikol::WINDOW_FLAGS_GFX_HARDWARE | nikol::WINDOW_FLAGS_RESIZABLE;
  nikol::Window* window = nikol::window_open("Batch Renderer", 1280, 720, win_flags);
  NIKOL_ASSERT(window, "Could not open a window");

  renderer_create(window);
 
  while(nikol::window_is_open(window)) {
    if(nikol::input_key_pressed(nikol::KEY_ESCAPE)) {
      break;
    }
  
    renderer_clear(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    renderer_begin();
    
    render_quad(glm::vec2(100.0f), glm::vec2(64.0f, 64.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    renderer_end();
    nikol::window_poll_events(window);
  }

  renderer_destroy();
  nikol::window_close(window);
  nikol::shutdown();
}
