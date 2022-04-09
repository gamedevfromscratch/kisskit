#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "kiss.h"

char *title = "SDL 14 - Animated Sprites";
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *sprite_sheet;
SDL_Rect sprite[] = {
	{ 0, 0, 100, 200 },
	{ 100, 0, 100, 200 },
	{ 200, 0, 100, 200 },
	{ 300, 0, 100, 200 },
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
	SDL_Texture *ret = NULL;
	SDL_Texture *texture = NULL;
	SDL_Surface *surface = NULL;
	surface = IMG_LOAD(path);
//	SDL_SETCOLORKEY(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x00, 0xFF, 0xFF));
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

int sprite_draw_ex(SDL_Rect *sprite, int x, int y, double zoom, double angle, SDL_RendererFlip flip, const SDL_Color *c)
{
	int status = -1;
	SDL_Rect target = { x, y, sprite->w * zoom, sprite->h * zoom };
	if (c) {
		SDL_SETTEXTURECOLORMOD(sprite_sheet, c->r, c->g, c->b);
		SDL_SETTEXTUREALPHAMOD(sprite_sheet, c->a);
	}
	SDL_RENDERCOPYEX(renderer, sprite_sheet, sprite, &target, angle, NULL, flip);
	if (c) {
		SDL_SETTEXTURECOLORMOD(sprite_sheet, 0xFF, 0xFF, 0xFF);
		SDL_SETTEXTUREALPHAMOD(sprite_sheet, 0xFF);
	}
	status = 0;
error:
	return status;
}

#define SPRITE_DRAW_EX(...) SDL_ZERO(sprite_draw_ex, __VA_ARGS__)

void close_sdl(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

double x = 0;
double y = 0;
double dx = 0;
double dy = 0;
double angle = 0;
SDL_Color tint = { 0xFF, 0xFF, 0xFF, 0xFF };

int flip = 0;

int frame = 0;

int draw(void)
{
	int status = -1;
	int w, h;
	double zoom = 1;
	SDL_GetWindowSize(window, &w, &h);
	SDL_RendererFlip sdl_flip = 0;

	SDL_SetRenderDrawColor(renderer, 0x7F, 0x7F, 0x7F, 0xFF);
	SDL_RENDERCLEAR(renderer);
	if (dx < 0)
		flip = 1;
	if (dx > 0)
		flip = 0;
	if (flip)
		sdl_flip = SDL_FLIP_HORIZONTAL;
	SPRITE_DRAW_EX(&sprite[(frame / 16) % NELEMS(sprite)], (int) x, (int) y, zoom, angle, sdl_flip, &tint);
	SDL_RenderPresent(renderer);
	if (dx > 4)
		dx = 4;
	if (dx < -4)
		dx = -4;
	if (dy > 4)
		dy = 4;
	if (dy < -4)
		dy = -4;
	if (dx || dy)
		frame += (int) hypot(dx, dy);
	x += dx * cos(RAD(angle)) - dy * sin(RAD(angle));
	y += dx * sin(RAD(angle)) + dy * cos(RAD(angle));
	status = 0;
error:
	return status;
}

#define DRAW(...) SDL_ZERO(draw, __VA_ARGS__)

void key_down(SDL_Keycode k)
{
	switch (k) {
	case SDLK_LEFT:
		dx -= 2;
		break;

	case SDLK_RIGHT:
		dx += 2;
		break;

	case SDLK_UP:
		dy -= 2;
		break;

	case SDLK_DOWN:
		dy += 2;
		break;

	case SDLK_q:
		tint.r += 4;
		break;

	case SDLK_w:
		tint.g += 4;
		break;

	case SDLK_e:
		tint.b += 4;
		break;

	case SDLK_r:
		tint.a += 4;
		break;

	case SDLK_a:
		tint.r -= 4;
		break;

	case SDLK_s:
		tint.g -= 4;
		break;

	case SDLK_d:
		tint.b -= 4;
		break;

	case SDLK_f:
		tint.a -= 4;
		break;

	case SDLK_t:
		angle += 15;
		break;

	case SDLK_g:
		angle -= 15;
		break;

	default:
		dx = 0;
		dy = 0;
		break;
	}
}

int main(void)
{
	int exit_status = EXIT_FAILURE;
	bool quit = 0;
	SDL_Event e;

	INIT_SDL();

	sprite_sheet = LOAD_TEXTURE("animation.png");

	while (!quit) {
		DRAW();
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = 1;
			else if (e.type == SDL_KEYDOWN)
				key_down(e.key.keysym.sym);
		}
	}

	exit_status = EXIT_SUCCESS;
error:
	SDL_DestroyTexture(sprite_sheet);
	close_sdl();
	exit(exit_status);
}
