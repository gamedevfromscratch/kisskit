#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "kiss.h"

char *title = "SDL 08 - Geometry Rendering";
SDL_Window *window;
SDL_Renderer *renderer;

int init_sdl(void)
{
	int status = -1;
	int x = SDL_WINDOWPOS_UNDEFINED;
	int y = SDL_WINDOWPOS_UNDEFINED;
	int width = 800;
	int height = 600;
	int flags = 0;		// SDL_WINDOW_FULLSCREEN_DESKTOP

	SDL_INIT(SDL_INIT_VIDEO);
	IMG_INIT(IMG_INIT_PNG);
	window = SDL_CREATEWINDOW(title, x, y, width, height, flags);
	renderer = SDL_CREATERENDERER(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	status = 0;
error:
	return status;
}

#define INIT_SDL(...) SDL_ZERO(init_sdl, __VA_ARGS__)

void close_sdl(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

int a = 0;

int draw(void)
{
	int status = -1;
	int w, h;
	int i;
	SDL_GetWindowSize(window, &w, &h);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RENDERCLEAR(renderer);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RENDERFILLRECT(renderer, &(SDL_Rect){w / 4, h / 4, w / 2, h / 2});
	SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RENDERDRAWRECT(renderer, &(SDL_Rect){w / 6, h / 6, w * 2 / 3, h * 2 / 3});
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	SDL_RENDERDRAWLINE(renderer, 0, a % h, w, h - a % h);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
	for (i = 0; i < h; i += 4) {
		SDL_RENDERDRAWPOINT(renderer, a % w, i);
	}
	SDL_RenderPresent(renderer);
	status = 0;
error:
	return status;
}

#define DRAW(...) SDL_ZERO(draw, __VA_ARGS__)

int main(void)
{
	int exit_status = EXIT_FAILURE;
	bool quit = 0;
	SDL_Event e;

	INIT_SDL();

	while (!quit) {
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				quit = 1;
		DRAW();
		a += 1;
	}

	exit_status = EXIT_SUCCESS;
error:
	close_sdl();
	exit(exit_status);
}
