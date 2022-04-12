#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "kiss.h"

char *title = "SDL 05 - Optimized Surface Loading and Soft Stretching";
SDL_Window *window;
SDL_Surface *screen;
SDL_Surface *image;

int init_sdl(void)
{
	int status = -1;
	int x = SDL_WINDOWPOS_UNDEFINED;
	int y = SDL_WINDOWPOS_UNDEFINED;
	int width = 1200;
	int height = 549;
	int flags = 0;		// SDL_WINDOW_FULLSCREEN_DESKTOP

	SDL_INIT(SDL_INIT_VIDEO);
	IMG_INIT(IMG_INIT_PNG);
	window = SDL_CREATEWINDOW(title, x, y, width, height, flags);
	screen = SDL_GETWINDOWSURFACE(window);
	status = 0;
error:
	return status;
}

#define INIT_SDL(...) SDL_ZERO(init_sdl, __VA_ARGS__)

void close_sdl(void)
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int draw(void)
{
	int status = -1;
	uint32_t colour = SDL_MapRGB(screen->format, 0xFF, 0x7F, 0x3F);
	SDL_Rect dstrect = { 0, 0, screen->w, screen->h };
	SDL_FILLRECT(screen, NULL, colour);
	SDL_BLITSCALED(image, NULL, screen, &dstrect);
	SDL_UPDATEWINDOWSURFACE(window);
	status = 0;
error:
	return status;
}

#define DRAW(...) SDL_ZERO(draw, __VA_ARGS__)

SDL_Surface *load_image(char *path, SDL_Surface *screen)
{
	SDL_Surface *surface2 = NULL;
	SDL_Surface *surface = NULL;
	surface = IMG_LOAD(path);
	surface2 = SDL_CONVERTSURFACE(surface, screen->format, 0);
error:
	SDL_FreeSurface(surface);
	return surface2;
}

#define LOAD_IMAGE(...) SDL_CHK(load_image, __VA_ARGS__)

int main(void)
{
	int exit_status = EXIT_FAILURE;
	bool quit = 0;
	SDL_Event e;

	INIT_SDL();

	image = LOAD_IMAGE("logo.png", screen);
	DRAW();

	while (!quit) {
		SDL_WAITEVENT(&e);
		if (e.type == SDL_QUIT)
			quit = 1;
	}

	exit_status = EXIT_SUCCESS;
error:
	SDL_FreeSurface(image);
	close_sdl();
	exit(exit_status);
}
