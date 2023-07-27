#include <stdio.h> /* printf and fprintf */
#include <iostream>
#include "render.h"
#include "glm/gtx/transform.hpp"

#include <SDL2/SDL.h>

/* Sets constants */

int main(int argc, char** argv)
{
	SDL_Window* window = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
		return 1;
	}
	window = SDL_CreateWindow("SDL Example", /* Title of the SDL window */
		SDL_WINDOWPOS_UNDEFINED, /* Position x of the window */
		SDL_WINDOWPOS_UNDEFINED, /* Position y of the window */
		WIDTH, /* Width of the window in pixels */
		HEIGHT, /* Height of the window in pixels */
		0); /* Additional flag(s) */
	if (window == NULL) {
		fprintf(stderr, "SDL window failed to initialise: %s\n", SDL_GetError());
		return 1;
	}
	SDL_Surface* window_surface = SDL_GetWindowSurface(window);
	if (!window_surface)
	{
		std::cout << "Failed to get the surface from the window\n";
		std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
		return -1;
	}

    /*
	glm::mat4 trans = glm::rotate(0.78f, glm::vec3(0, 0, 1));
	trans = glm::rotate(trans,-0.9f, glm::vec3(1, 0, 0));

	for (auto &sphere : objects) {
		glm::vec4 ful_vec = glm::vec4(sphere.m_position, 1);
		glm::vec4 ful_trans = ful_vec*trans;
		sphere.m_position = { ful_trans.x,ful_trans.y,ful_trans.z };
	}
*/

    bool keep_window_open = true;
	while (keep_window_open)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) > 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				keep_window_open = false;
				break;
			}
		}
		SDL_LockSurface(window_surface);

		render(static_cast<uint32_t*>(window_surface->pixels));
		/* Unlock the surface */
		SDL_UnlockSurface(window_surface);

		SDL_UpdateWindowSurface(window);
	}

	SDL_DestroyWindow(window);

	/* Shuts down all SDL subsystems */
	SDL_Quit();

	return 0;
}
