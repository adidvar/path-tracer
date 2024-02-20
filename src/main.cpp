#include <SDL2/SDL.h>
#include <stdio.h> /* printf and fprintf */

#include <PathTracer/pathtracer.hpp>
#include <iostream>

#define WIDTH 1280
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

  bool keep_window_open = true;
  while (keep_window_open) {
    SDL_Event e;
    while (SDL_PollEvent(&e) > 0) {
      switch (e.type) {
        case SDL_QUIT:
          keep_window_open = false;
          break;
      }
    }
    SDL_LockSurface(window_surface);

    renderer.Trace(10);
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
