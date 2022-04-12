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

char *title = "SDL 24 - Calculating Frame Rate";
SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *font1;
TTF_Font *font2;
SDL_Texture *text1;
SDL_Texture *text2;
SDL_Texture *text3;
int n_frames;
double fps;

struct timer {
	uint32_t start;
	uint32_t pause;
};

struct timer _fps_timer = { 0, 0 };

struct timer *fps_timer = &_fps_timer;

void timer_start(struct timer *t)
{
	t->start = SDL_GetTicks();
	t->pause = 0;
}

void timer_stop(struct timer *t)
{
	t->start = 0;
	t->pause = 0;
}

void timer_pause(struct timer *t)
{
	if (!t->start || t->pause)
		return;
	t->pause = SDL_GetTicks() - t->start;
}

void timer_unpause(struct timer *t)
{
	if (!t->pause)
		return;
	t->start = SDL_GetTicks() - t->pause;
	t->pause = 0;
}

uint32_t timer_get(struct timer *t)
{
	if (!t->start)
		return 0;
	if (t->pause)
		return t->pause;
	return SDL_GetTicks() - t->start;
}

bool timer_is_started(struct timer *t)
{
	return t->start && 1;
}

bool timer_is_paused(struct timer *t)
{
	return t->pause && 1;
}

int init_sdl(void)
{
	int status = -1;

	SDL_INIT(SDL_INIT_VIDEO);
	IMG_INIT(IMG_INIT_PNG);
	TTF_INIT();
	window = SDL_CREATEWINDOW(title, 50, 50, 800, 600, SDL_WINDOW_FULLSCREEN_DESKTOP);
	renderer = SDL_CREATERENDERER(window, -1, SDL_RENDERER_ACCELERATED);	// | SDL_RENDERER_PRESENTVSYNC);
	status = 0;
error:
	return status;
}

#define INIT_SDL(...) SDL_ZERO(init_sdl, __VA_ARGS__)

SDL_Texture *texture_from_text(TTF_Font *font, char *text, SDL_Color colour)
{
	SDL_Texture *texture = NULL;
	SDL_Surface *surface = NULL;
	surface = TTF_RENDERTEXT_BLENDED(font, text, colour);
	texture = SDL_CREATETEXTUREFROMSURFACE(renderer, surface);
error:
	SDL_FreeSurface(surface);
	return texture;
}

#define TEXTURE_FROM_TEXT(...) SDL_CHK(texture_from_text, __VA_ARGS__)

void close_sdl(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int draw(void)
{
	int status = -1;
	char time_text[20];

	int w, h;
	int tw, th;
	SDL_Rect target;

	snprintf(time_text, sizeof(time_text), "%.4f", fps);

	text2 = TEXTURE_FROM_TEXT(font2, time_text, (SDL_Color){0xFF, 0x7F, 0xCF, 0xFF});

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x7F, 0xFF);
	SDL_RENDERCLEAR(renderer);

	SDL_GetWindowSize(window, &w, &h);
	SDL_QUERYTEXTURE(text1, NULL, NULL, &tw, &th);
	target = (SDL_Rect) {
	w / 2 - tw / 2, 10, tw, th};
	SDL_RENDERCOPY(renderer, text1, NULL, &target);
	SDL_QUERYTEXTURE(text2, NULL, NULL, &tw, &th);
	target = (SDL_Rect) {
	w / 2 - tw / 2, h / 2 - th / 2, tw, th};
	SDL_RENDERCOPY(renderer, text2, NULL, &target);
	SDL_QUERYTEXTURE(text3, NULL, NULL, &tw, &th);
	target = (SDL_Rect) {
	w / 2 - tw / 2, h - th - 10, tw, th};
	SDL_RENDERCOPY(renderer, text3, NULL, &target);

	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(text2);

	status = 0;
error:
	return status;
}

#define DRAW(...) SDL_ZERO(draw, __VA_ARGS__)

int main(void)
{
	int exit_status = EXIT_FAILURE;
	bool quit = 0;
	SDL_Event e;

	INIT_SDL();

	font1 = TTF_OPENFONT("OpenSans-Regular.ttf", 28);
	font2 = TTF_OPENFONT("OpenSans-Regular.ttf", 100);
	text1 = TEXTURE_FROM_TEXT(font1, "Average Frames Per Second:", (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF});
	text3 = TEXTURE_FROM_TEXT(font2, "BOGOMETER", (SDL_Color){0xFF, 0x7F, 0x3F, 0xFF});

	DRAW();

	timer_start(fps_timer);

	while (!quit) {
		DRAW();
		n_frames++;
		fps = n_frames / (timer_get(fps_timer) / 1000.0);
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = 1;
		}
	}

	exit_status = EXIT_SUCCESS;
error:
	SDL_DestroyTexture(text1);
	SDL_DestroyTexture(text3);
	TTF_CloseFont(font1);
	TTF_CloseFont(font2);
	close_sdl();
	exit(exit_status);
}
