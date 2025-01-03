#include <nikol/nikol_core.hpp>

#include "camera.h"
#include "mesh.h"
#include "renderer.h"
#include "trasform.h"

int main() {
  // Initialze the library
  if(!nikol::init()) {
    return -1;
  }

  // Openinig the window
  nikol::i32 win_flags = nikol::WINDOW_FLAGS_FOCUS_ON_CREATE | nikol::WINDOW_FLAGS_GFX_HARDWARE;
  nikol::Window* window = nikol::window_open("Mesh Viewer", 1366, 768, win_flags);
  if(!window) {
    return -1;
  }

  Renderer* renderer = renderer_create(window);
  nikol::GfxContext* gfx = renderer_get_gfx_context(renderer);

  Mesh* mesh = mesh_create(gfx, MESH_TYPE_CUBE);
  Transform transform = transform_create(glm::vec3(10.0f, 0.0f, 5.0f), glm::vec3(1.0f));

  Camera camera = camera_create(glm::vec3(10.0f, 0.0f, 10.0f), glm::vec3(-3.0f, 0.0f, 0.0f));

  // Main loop
  while(nikol::window_is_open(window)) {
    // Will stop the application when F1 is pressed
    if(nikol::input_key_pressed(nikol::KEY_ESCAPE)) {
      break;
    }
   
    camera_update(camera, window);

    renderer_clear(renderer, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    renderer_begin(renderer, camera);

    // render_mesh(renderer, mesh, nullptr, transform);

    renderer_end(renderer);
    
    // Poll the window events
    nikol::window_poll_events(window);
  }

  // De-initialze
  renderer_destroy(renderer); 
  nikol::window_close(window);
  nikol::shutdown();
}
