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

char *title = "SDL 16 - TrueType Fonts";
SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *font1;
TTF_Font *font2;
SDL_Texture *text1;
SDL_Texture *text2;

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
	TTF_INIT();
	window = SDL_CREATEWINDOW(title, x, y, width, height, flags);
	renderer = SDL_CREATERENDERER(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

int x = 0;
int y = 0;

int draw(void)
{
	int status = -1;
	int w, h;
	int tw, th;
	int i;
	SDL_Rect target;

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x7F, 0xFF);
	SDL_RENDERCLEAR(renderer);

	SDL_GetWindowSize(window, &w, &h);
	SDL_QUERYTEXTURE(text1, NULL, NULL, &tw, &th);
	for (i = -3; i < 4; ++i) {
		target = (SDL_Rect) {
		w / 2 - tw / 2 + x + 4 * i, h / 2 - th / 2 + th * i + y, tw, th};
		SDL_RENDERCOPY(renderer, text1, NULL, &target);
	}
	SDL_QUERYTEXTURE(text2, NULL, NULL, &tw, &th);
	target = (SDL_Rect) {
	w / 2 - tw / 2 - x, h - th - 10 - y, tw, th};
	SDL_RENDERCOPY(renderer, text2, NULL, &target);
	target = (SDL_Rect) {
	w / 2 - tw / 2 - x, 10 - y, tw, th};
	SDL_RENDERCOPY(renderer, text2, NULL, &target);

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
		x -= 4;
		break;

	case SDLK_RIGHT:
		x += 4;
		break;

	case SDLK_UP:
		y -= 4;
		break;

	case SDLK_DOWN:
		y += 4;
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

	font1 = TTF_OPENFONT("OpenSans-Regular.ttf", 28);
	font2 = TTF_OPENFONT("OpenSans-Regular.ttf", 112);
	text1 = TEXTURE_FROM_TEXT(font1, "The quick brown fox jumps over the lazy dog", (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF});
	text2 = TEXTURE_FROM_TEXT(font2, "BOGOTRON", (SDL_Color){0xFF, 0x7F, 0xCF, 0xFF});

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
	SDL_DestroyTexture(text1);
	SDL_DestroyTexture(text2);
	TTF_CloseFont(font1);
	TTF_CloseFont(font2);
	close_sdl();
	exit(exit_status);
}
