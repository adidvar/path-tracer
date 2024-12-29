#include <SDL2/SDL.h>
#include <stdio.h> /* printf and fprintf */

#include <chrono>
#include <iostream>
#include <render.hpp>

#define WIDTH 480
#define HEIGHT 480

int SDL_main(int argc, char* argv[]) {
  Renderer renderer(WIDTH, HEIGHT);

  SDL_Window* window = NULL;
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
    return 1;
  }
  window =
      SDL_CreateWindow("Path tracing",          /* Title of the SDL window */
                       SDL_WINDOWPOS_UNDEFINED, /* Position x of the window */
                       SDL_WINDOWPOS_UNDEFINED, /* Position y of the window */
                       WIDTH,  /* Width of the window in pixels */
                       HEIGHT, /* Height of the window in pixels */
                       0);     /* Additional flag(s) */
  if (window == NULL) {
    fprintf(stderr, "SDL window failed to initialise: %s\n", SDL_GetError());
    return 1;
  }
  SDL_Surface* window_surface = SDL_GetWindowSurface(window);
  if (!window_surface) {
    std::cout << "Failed to get the surface from the window\n";
    std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
    return -1;
  }

  bool move_enabled = false;
  glm::vec2 start_pos;
  float speed = 0.01;

  bool keep_window_open = true;
  while (keep_window_open) {
    SDL_Event event;
    while (SDL_PollEvent(&event) > 0) {
      switch (event.type) {
        case SDL_QUIT:
          keep_window_open = false;
          break;
        case SDL_MOUSEMOTION:
          glm::vec2 end_pos = glm::vec2(event.motion.x, event.motion.y);
          if (move_enabled) {
            renderer.m_camera.Rotate(speed * (end_pos - start_pos));
            renderer.Clear();
          }
          start_pos = end_pos;
          break;

        case SDL_MOUSEWHEEL:
          if (event.wheel.y > 0) {
            std::cout << "Колесико: Прокрутка вгору" << std::endl;
          } else if (event.wheel.y < 0) {
            std::cout << "Колесико: Прокрутка вниз" << std::endl;
          }
          renderer.Clear();
          break;

        case SDL_MOUSEBUTTONDOWN:
          if (event.button.button == SDL_BUTTON_LEFT) move_enabled = true;
          break;

        case SDL_MOUSEBUTTONUP:
          if (event.button.button == SDL_BUTTON_LEFT) move_enabled = false;
          renderer.Clear();
          break;

        default:
          break;
      }
    }
    SDL_LockSurface(window_surface);
    using namespace std::chrono;

    auto begin = high_resolution_clock::now();

    renderer.Trace(1);

    auto end = high_resolution_clock::now();

    std::cout << "time: "
              << (duration_cast<duration<float, std::ratio<1, 1>>>(end - begin)
                      .count())
              << std::endl;

    renderer.Export(static_cast<uint32_t*>(window_surface->pixels));
    /* Unlock the surface */
    SDL_UnlockSurface(window_surface);

    SDL_UpdateWindowSurface(window);
  }

  SDL_DestroyWindow(window);

  /* Shuts down all SDL subsystems */
  SDL_Quit();

  return 0;
}
