#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "kiss.h"

char *title = "SDL 07 - Texture Loading and Rendering";
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;

int init_sdl(void)
{
	int status = -1;
	int x = SDL_WINDOWPOS_UNDEFINED;
	int y = SDL_WINDOWPOS_UNDEFINED;
	int width = 800;
	int height = 366;
	int flags = 0;		// SDL_WINDOW_FULLSCREEN_DESKTOP

	SDL_INIT(SDL_INIT_VIDEO);
	IMG_INIT(IMG_INIT_PNG);
	window = SDL_CREATEWINDOW(title, x, y, width, height, flags);
	renderer = SDL_CREATERENDERER(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x7F, 0x3F, 0xFF);
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
	SDL_GetWindowSize(window, &w, &h);
	SDL_RENDERCLEAR(renderer);
//	SDL_RENDERCOPY(renderer, texture, NULL, NULL);
	SDL_RENDERCOPYEX(renderer, texture, NULL, NULL, a, NULL, 0);
	SDL_RenderPresent(renderer);
	status = 0;
error:
	return status;
}

#define DRAW(...) SDL_ZERO(draw, __VA_ARGS__)

SDL_Texture *load_texture(char *path)
{
	SDL_Texture *texture = NULL;
	SDL_Surface *surface = NULL;
	surface = IMG_LOAD(path);
	texture = SDL_CREATETEXTUREFROMSURFACE(renderer, surface);
error:
	SDL_FreeSurface(surface);
	return texture;
}

#define LOAD_TEXTURE(...) SDL_CHK(load_texture, __VA_ARGS__)

int main(void)
{
	int exit_status = EXIT_FAILURE;
	bool quit = 0;
	SDL_Event e;

	INIT_SDL();

	texture = LOAD_TEXTURE("logo.png");

	while (!quit) {
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = 1;
		}
		DRAW();
//		SDL_Delay(1000/60);
		a += 1;
	}

	exit_status = EXIT_SUCCESS;
error:
	SDL_DestroyTexture(texture);
	close_sdl();
	exit(exit_status);
}
