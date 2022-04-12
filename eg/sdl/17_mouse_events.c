#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "kiss.h"

char *title = "SDL 17 - Mouse Events";
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *image;
int width, height;
int sel_x = -1, sel_y = -1;
int sel_solid = 0;
int need_clicks = 2;

bool quit = 0;
bool done = 0;
bool next = 0;

enum {
	tiles_w = 4,
	tiles_h = 4
};

uint8_t clicks[tiles_h][tiles_w];

int texture_draw_ex(SDL_Texture *texture, SDL_Rect *clip, int x, int y, double zoom, double angle, SDL_RendererFlip flip, const SDL_Color *c)
{
	int status = -1;
	SDL_Rect target = { x, y, clip->w * zoom, clip->h * zoom };
	if (c) {
		SDL_SETTEXTURECOLORMOD(texture, c->r, c->g, c->b);
		SDL_SETTEXTUREALPHAMOD(texture, c->a);
	}
	SDL_RENDERCOPYEX(renderer, texture, clip, &target, angle, NULL, flip);
	if (c) {
		SDL_SETTEXTURECOLORMOD(texture, 0xFF, 0xFF, 0xFF);
		SDL_SETTEXTUREALPHAMOD(texture, 0xFF);
	}
	status = 0;
error:
	return status;
}

#define TEXTURE_DRAW_EX(...) SDL_ZERO(texture_draw_ex, __VA_ARGS__)

int draw(void)
{
	int status = -1;
	int x, y;

	int done1 = 1;

	SDL_SetRenderDrawColor(renderer, 0x7F, 0x7F, 0x7F, 0xFF);
	SDL_RENDERCLEAR(renderer);

	for (y = 0; y < tiles_h; ++y) {
		for (x = 0; x < tiles_w; ++x) {
			int xp = width * x / tiles_w;
			int yp = height * y / tiles_h;
			int xp1 = width * (x + 1) / tiles_w;
			int yp1 = height * (y + 1) / tiles_h;
			int c = clicks[y][x];
			SDL_Rect clip = { xp, yp, xp1 - xp, yp1 - yp };

			if (x == sel_x && y == sel_y) {
				if (sel_solid) {
					SDL_SetRenderDrawColor(renderer, 0xFF, 0x7F, 0x3F, 0xFF);
					SDL_RENDERFILLRECT(renderer, &clip);
				} else {
					SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
					SDL_RENDERDRAWRECT(renderer, &clip);
				}
			}
			if (c > need_clicks) {
				c = clicks[y][x] = 0;
			}
			if (c < need_clicks) {
				done1 = 0;
			}
			if (c) {
				int a = 0xFF * c / need_clicks;
				TEXTURE_DRAW_EX(image, &clip, xp, yp, 1, 0, 0, &(SDL_Color){0xFF, 0xFF, 0xFF, a});
			}
		}
	}
	SDL_RenderPresent(renderer);
	done = done1;
	status = 0;
error:
	return status;
}

#define DRAW(...) SDL_ZERO(draw, __VA_ARGS__)

bool changed = 1;

void mouse_event(SDL_Event *e)
{
	int x, y;

	if (e->type == SDL_MOUSEMOTION) {
		x = e->motion.x * tiles_w / width;
		y = e->motion.y * tiles_h / height;
		if (x != sel_x || y != sel_y) {
			sel_x = x;
			sel_y = y;
			sel_solid = 0;
			changed = 1;
		}
	} else if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
		x = e->button.x * tiles_w / width;
		y = e->button.y * tiles_h / height;
		if (x >= 0 && x < tiles_w && y >= 0 && y < tiles_h) {
			if (e->type == SDL_MOUSEBUTTONUP) {
				clicks[y][x]++;
				sel_x = -1;
				sel_y = -1;
			} else {
				sel_x = x;
				sel_y = y;
				sel_solid = 1;
			}
			changed = 1;
		}
	}
//	buttons = SDL_GetMouseState(&x, &y);
}

void key_down(SDL_Keycode k)
{
	switch (k) {
	case SDLK_SPACE:
		if (done)
			next = 1;
		break;

	case SDLK_ESCAPE:
		next = 1;
		break;
	}
}

int play(char *image_file)
{
	int status = -1;
	SDL_Event e;
	SDL_Surface *surface = NULL;

	done = 0;
	next = 0;
	changed = 1;

	memset(clicks, 0, sizeof(clicks));

	surface = IMG_LOAD(image_file);
	width = surface->w;
	height = surface->h;
	window = SDL_CREATEWINDOW(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	renderer = SDL_CREATERENDERER(window, -1, SDL_RENDERER_ACCELERATED);
	image = SDL_CREATETEXTUREFROMSURFACE(renderer, surface);
	SDL_FREESURFACE(surface);
	SDL_SETTEXTUREBLENDMODE(image, SDL_BLENDMODE_BLEND);

	while (!quit && !next) {
		if (changed) {
			DRAW();
			changed = 0;
		}
		SDL_WaitEvent(&e);
		if (e.type == SDL_QUIT)
			quit = 1;
		if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			mouse_event(&e);
		if (e.type == SDL_KEYDOWN)
			key_down(e.key.keysym.sym);
	}

	status = 0;
error:
	SDL_DESTROYTEXTURE(image);
	SDL_DESTROYRENDERER(renderer);
	SDL_DESTROYWINDOW(window);
	SDL_FREESURFACE(surface);
	return status;
}

#define PLAY(...) SDL_ZERO(play, __VA_ARGS__)

int main(int argc, char *argv[])
{
	int exit_status = EXIT_FAILURE;

	--argc;
	++argv;

	srand((unsigned) time(NULL));

	SDL_INIT(SDL_INIT_VIDEO);
	IMG_INIT(IMG_INIT_PNG | IMG_INIT_JPG);

	while (!quit) {
		char *image_file = "cat.jpg";
		if (argc > 0) {
			int i = rand() % argc;
			image_file = argv[i];
			for (; i <= argc; ++i)
				argv[i] = argv[i + 1];
			--argc;
		}
		PLAY(image_file);
	}

	exit_status = EXIT_SUCCESS;
error:
	IMG_Quit();
	SDL_Quit();
	exit(exit_status);
}
