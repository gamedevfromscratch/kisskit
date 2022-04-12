#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "kiss.h"

char *title = "SDL 11 - Sprite Sheets";
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *sprite_sheet;
SDL_Rect sprite[4] = {
	{ 0, 0, 100, 100 },
	{ 100, 0, 100, 100 },
	{ 0, 100, 100, 100 },
	{ 100, 100, 100, 100 },
};

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

SDL_Texture *load_texture(char *path)
{
	SDL_Texture *texture = NULL;
	SDL_Surface *surface = NULL;
	surface = IMG_LOAD(path);
	SDL_SETCOLORKEY(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x00, 0xFF, 0xFF));
	texture = SDL_CREATETEXTUREFROMSURFACE(renderer, surface);
error:
	SDL_FreeSurface(surface);
	return texture;
}

#define LOAD_TEXTURE(...) SDL_CHK(load_texture, __VA_ARGS__)

int sprite_draw(SDL_Rect *sprite, int x, int y)
{
	int status = -1;
	SDL_Rect target = { x, y, sprite->w, sprite->h };
	SDL_RENDERCOPY(renderer, sprite_sheet, sprite, &target);
	status = 0;
error:
	return status;
}

#define SPRITE_DRAW(...) SDL_ZERO(sprite_draw, __VA_ARGS__)

void close_sdl(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

int x = 0;
int y = 0;
int d = 4;

int draw(void)
{
	int status = -1;
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RENDERCLEAR(renderer);
	SPRITE_DRAW(&sprite[0], x, y);
	SPRITE_DRAW(&sprite[1], w - sprite[1].w - x, y);
	SPRITE_DRAW(&sprite[2], x, h - sprite[2].h - y);
	SPRITE_DRAW(&sprite[3], w - sprite[3].w - x, h - sprite[3].h - y);
	SDL_RenderPresent(renderer);
	status = 0;
error:
	return status;
}

#define DRAW(...) SDL_ZERO(draw, __VA_ARGS__)

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

	sprite_sheet = LOAD_TEXTURE("sprite_sheet.png");

	DRAW();
	while (!quit) {
		SDL_WAITEVENT(&e);
		if (e.type == SDL_QUIT)
			quit = 1;
		else if (e.type == SDL_KEYDOWN) {
			key_down(e.key.keysym.sym);
			DRAW();
		}
	}

	exit_status = EXIT_SUCCESS;
error:
	SDL_DestroyTexture(sprite_sheet);
	close_sdl();
	exit(exit_status);
}
