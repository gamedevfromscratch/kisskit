#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "kiss.h"

char *title = "SDL 04 - Key Presses";
SDL_Window *window;
SDL_Surface *screen;
SDL_Surface *image;

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

int draw(int x, int y)
{
	int status = -1;
	uint32_t colour = SDL_MapRGB(screen->format, 0xFF, 0x7F, 0x3F);
	SDL_Rect dstrect = { x, y, image->w, image->h };
	SDL_FILLRECT(screen, NULL, colour);
	SDL_BLITSURFACE(image, NULL, screen, &dstrect);
	SDL_UPDATEWINDOWSURFACE(window);
	status = 0;
error:
	return status;
}

#define DRAW(...) SDL_ZERO(draw, __VA_ARGS__)

int x = 0;
int y = 0;
int d = 4;

void key_down(SDL_Keycode k)
{
	switch (k) {
	case SDLK_LEFT:
		x -= d;
		break;

	case SDLK_RIGHT:
		x += d;
		break;

	case SDLK_UP:
		y -= d;
		break;

	case SDLK_DOWN:
		y += d;
		break;

	default:
		x = 0;
		y = 0;
		break;
	}
}

int main(void)
{
	int exit_status = EXIT_FAILURE;
	bool quit = 0;
	SDL_Event e;

	INIT_SDL();

	image = IMG_LOAD("logo.png");

	while (!quit) {
		DRAW(x, y);
		SDL_WAITEVENT(&e);
		if (e.type == SDL_QUIT)
			quit = 1;
		else if (e.type == SDL_KEYDOWN)
			key_down(e.key.keysym.sym);
	}

	exit_status = EXIT_SUCCESS;
error:
	SDL_FreeSurface(image);
	close_sdl();
	exit(exit_status);
}
