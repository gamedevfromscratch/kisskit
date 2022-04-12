#include <SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int main(void)
{
	int exit_status = EXIT_FAILURE;
	char *title = "SDL Demo";
	SDL_Window *window;
	SDL_Renderer *renderer;
	bool quit = 0;
	SDL_Event e;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(title, 50, 50, 800, 600, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
		goto error;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	lineRGBA(renderer, 100, 100, 300, 200, 255, 255, 255, 255);
	circleRGBA(renderer, 300, 200, 50, 255, 255, 255, 255);

	SDL_RenderPresent(renderer);

	while (!quit) {
		SDL_WaitEvent(&e);
		if (e.type == SDL_QUIT)
			quit = 1;
	}

	exit_status = EXIT_SUCCESS;
error:
	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(exit_status);
}
