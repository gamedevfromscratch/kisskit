#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "kiss.h"

char *title = "SDL 02 - Getting an Image on the Screen";
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

int main(void)
{
	int exit_status = EXIT_FAILURE;

	INIT_SDL();

	image = IMG_LOAD("logo.png");
	SDL_BLITSURFACE(image, NULL, screen, NULL);
	SDL_UPDATEWINDOWSURFACE(window);

	SDL_Delay(2000);

	exit_status = EXIT_SUCCESS;
error:
	SDL_FreeSurface(image);
	close_sdl();
	exit(exit_status);
}
