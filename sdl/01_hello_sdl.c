#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "kiss.h"

char *title = "SDL 01 - Hello SDL";
SDL_Window *window;
SDL_Surface *screen;

int main(void)
{
	int exit_status = EXIT_FAILURE;
	int x = SDL_WINDOWPOS_UNDEFINED;
	int y = SDL_WINDOWPOS_UNDEFINED;
	int width = 800;
	int height = 600;
	int flags = 0;		// SDL_WINDOW_FULLSCREEN_DESKTOP
	uint32_t colour;

	SDL_INIT(SDL_INIT_VIDEO);
	window = SDL_CREATEWINDOW(title, x, y, width, height, flags);
	screen = SDL_GETWINDOWSURFACE(window);

	colour = SDL_MapRGB(screen->format, 0xFF, 0x7F, 0x3F);
	SDL_FILLRECT(screen, NULL, colour);
	SDL_UPDATEWINDOWSURFACE(window);

	SDL_Delay(2000);

	exit_status = EXIT_SUCCESS;
error:
	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(exit_status);
}
