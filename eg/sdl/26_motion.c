#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "kiss.h"

char *title = "SDL 26 - Motion";
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *obj_texture;

enum { obj_vel = 10 };

struct obj {
	int x, y;
	int vx, vy;
};

int init_sdl(void)
{
	int status = -1;

	SDL_INIT(SDL_INIT_VIDEO);
	IMG_INIT(IMG_INIT_PNG);
	window = SDL_CREATEWINDOW(title, 50, 50, 800, 600, 0);
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

SDL_Texture *load_texture(char *path)
{
	SDL_Texture *ret = NULL;
	SDL_Texture *texture = NULL;
	SDL_Surface *surface = NULL;
	surface = IMG_LOAD(path);
	SDL_SETCOLORKEY(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x00, 0xFF, 0xFF));
	texture = SDL_CREATETEXTUREFROMSURFACE(renderer, surface);
	SDL_SETTEXTUREBLENDMODE(texture, SDL_BLENDMODE_BLEND);
	ret = texture;
error:
	if (!ret)
		SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	return ret;
}

#define LOAD_TEXTURE(...) SDL_CHK(load_texture, __VA_ARGS__)

void obj_handle_event(struct obj *obj, SDL_Event *e)
{
	if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
		switch (e->key.keysym.sym) {
		case SDLK_UP:
			obj->vy -= obj_vel;
			break;

		case SDLK_DOWN:
			obj->vy += obj_vel;
			break;

		case SDLK_LEFT:
			obj->vx -= obj_vel;
			break;

		case SDLK_RIGHT:
			obj->vx += obj_vel;
			break;
		}
	} else if (e->type == SDL_KEYUP && e->key.repeat == 0) {
		switch (e->key.keysym.sym) {
		case SDLK_UP:
			obj->vy += obj_vel;
			break;

		case SDLK_DOWN:
			obj->vy -= obj_vel;
			break;

		case SDLK_LEFT:
			obj->vx += obj_vel;
			break;

		case SDLK_RIGHT:
			obj->vx -= obj_vel;
			break;
		}
	}
}

int obj_move(struct obj *obj)
{
	int status = -1;
	int w, h;
	int tw, th;

	SDL_GetWindowSize(window, &w, &h);
	SDL_QUERYTEXTURE(obj_texture, NULL, NULL, &tw, &th);

	obj->x += obj->vx;
	obj->y += obj->vy;

	if (obj->x < 0 || obj->x + tw > w)
		obj->x -= obj->vx;

	if (obj->y < 0 || obj->y + th > h)
		obj->y -= obj->vy;

	status = 0;
error:
	return status;
}

#define OBJ_MOVE(...) SDL_ZERO(obj_move, __VA_ARGS__)

int obj_draw(struct obj *obj)
{
	int status = -1;
	int w, h;
	int tw, th;
	SDL_Rect target;

	SDL_GetWindowSize(window, &w, &h);
	SDL_QUERYTEXTURE(obj_texture, NULL, NULL, &tw, &th);

	target = (SDL_Rect) {
	obj->x, obj->y, tw, th};
	SDL_RENDERCOPY(renderer, obj_texture, NULL, &target);

	status = 0;
error:
	return status;
}

#define OBJ_DRAW(...) SDL_ZERO(obj_draw, __VA_ARGS__)

int main(void)
{
	int exit_status = EXIT_FAILURE;
	bool quit = 0;
	SDL_Event e;

	struct obj obj = { 0, 0, 0, 0 };

	INIT_SDL();

	obj_texture = LOAD_TEXTURE("foo.png");

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = 1;
			obj_handle_event(&obj, &e);
		}

		OBJ_MOVE(&obj);

		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x7F, 0xFF);
		SDL_RENDERCLEAR(renderer);
		OBJ_DRAW(&obj);
		SDL_RenderPresent(renderer);
	}

	exit_status = EXIT_SUCCESS;
error:
	SDL_DestroyTexture(obj_texture);
	close_sdl();
	exit(exit_status);
}
