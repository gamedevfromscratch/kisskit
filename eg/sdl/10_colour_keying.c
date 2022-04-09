#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "kiss.h"

struct texture {
	SDL_Texture *sdl_texture;
	int w;
	int h;
};

char *title = "SDL 10 - Colour Keying";
SDL_Window *window;
SDL_Renderer *renderer;
struct texture foo_texture;
struct texture back_texture;

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
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x7F, 0x3F, 0xFF);
	status = 0;
error:
	return status;
}

#define INIT_SDL(...) SDL_ZERO(init_sdl, __VA_ARGS__)

int texture_load(struct texture *t, char *path)
{
	SDL_Surface *surface = NULL;
	int status = -1;
	assert(t->sdl_texture == NULL);
	surface = IMG_LOAD(path);
	SDL_SETCOLORKEY(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x00, 0xFF, 0xFF));
	t->sdl_texture = SDL_CREATETEXTUREFROMSURFACE(renderer, surface);
	t->w = surface->w;
	t->h = surface->h;
	status = 0;
error:
	SDL_FreeSurface(surface);
	return status;
}

#define TEXTURE_LOAD(...) SDL_ZERO(texture_load, __VA_ARGS__)

void texture_free(struct texture *t)
{
	if (t->sdl_texture == NULL)
		return;
	SDL_DestroyTexture(t->sdl_texture);
	t->sdl_texture = NULL;
	t->w = 0;
	t->h = 0;
}

int texture_draw(struct texture *t, int x, int y)
{
	int status = -1;
	SDL_RENDERCOPY(renderer, t->sdl_texture, NULL, &(SDL_Rect){x, y, t->w, t->h});
	status = 0;
error:
	return status;
}

#define TEXTURE_DRAW(...) SDL_ZERO(texture_draw, __VA_ARGS__)

void close_sdl(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

int ox = 240;
int oy = 240;
int x;
int y;
int d = 4;

int draw(void)
{
	int status = -1;

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RENDERCLEAR(renderer);
	TEXTURE_DRAW(&back_texture, 0, 0);
	TEXTURE_DRAW(&foo_texture, x, y);
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
		x = ox;
		y = oy;
		break;
	}
}

int main(void)
{
	int exit_status = EXIT_FAILURE;
	bool quit = 0;
	SDL_Event e;

	INIT_SDL();

	TEXTURE_LOAD(&back_texture, "back.png");
	TEXTURE_LOAD(&foo_texture, "foo.png");

	x = ox;
	y = oy;

	while (!quit) {
		DRAW();
		SDL_WAITEVENT(&e);
		if (e.type == SDL_QUIT)
			quit = 1;
		else if (e.type == SDL_KEYDOWN)
			key_down(e.key.keysym.sym);
	}

	exit_status = EXIT_SUCCESS;
error:
	texture_free(&back_texture);
	texture_free(&foo_texture);
	close_sdl();
	exit(exit_status);
}
