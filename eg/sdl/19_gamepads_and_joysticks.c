#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "kiss.h"

char *title = "SDL 19 - Gamepads and Joysticks";
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *back;
SDL_Texture *sprite_sheet;
SDL_Rect sprite[] = {
	{ 0, 0, 100, 200 },
	{ 100, 0, 100, 200 },
	{ 200, 0, 100, 200 },
	{ 300, 0, 100, 200 },
};

int joy_dead_zone = 2000;
int n_joys = 0;
enum { n_joys_max = 4, n_axes_max = 6 };
SDL_Joystick *joy[n_joys_max];

double joy_x, joy_y;
double joy_d, joy_a;

double x = 240;
double y = 240;
double dx = 0;
double dy = 0;
double angle = 0;
SDL_Color tint = { 0xFF, 0xFF, 0xFF, 0xFF };

int flip = 0;

int frame = 0;

int init_sdl(void)
{
	int status = -1;
	int x = SDL_WINDOWPOS_UNDEFINED;
	int y = SDL_WINDOWPOS_UNDEFINED;
	int width = 800;
	int height = 600;
	int flags = 0;		// SDL_WINDOW_FULLSCREEN_DESKTOP
	int i;

	SDL_INIT(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	SDL_SETHINT(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	IMG_INIT(IMG_INIT_PNG);
	window = SDL_CREATEWINDOW(title, x, y, width, height, flags);
	renderer = SDL_CREATERENDERER(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	n_joys = SDL_NUMJOYSTICKS();
	if (n_joys == 0) {
		INFO("No joysticks");
	} else if (n_joys > n_joys_max) {
		INFO("More than %d joysticks: %d", n_joys_max, n_joys);
		n_joys = n_joys_max;
	} else {
		INFO("Found %d joysticks", n_joys);
	}
	for (i = 0; i < n_joys; ++i)
		joy[i] = SDL_JOYSTICKOPEN(i);
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
	int i;
	for (i = 0; i < n_joys; ++i)
		SDL_JOYSTICKCLOSE(joy[i]);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

int draw(void)
{
	int status = -1;
	int w, h;
	double zoom = 1;
	SDL_GetWindowSize(window, &w, &h);
	SDL_RendererFlip sdl_flip = 0;

	SDL_RENDERCOPY(renderer, back, NULL, NULL);
	if (flip)
		sdl_flip = SDL_FLIP_HORIZONTAL;
	angle = joy_a;
	if (flip)
		angle -= 180;
	SPRITE_DRAW_EX(&sprite[(frame / 16) % NELEMS(sprite)], (int) x, (int) y, zoom, angle, sdl_flip, &tint);
	SDL_RenderPresent(renderer);
	double d = hypot(dx, dy);
	if (d > 4) {
		dx *= 4 / d;
		dy *= 4 / d;
		d = 4;
	}
	if (dx || dy)
		frame += (int) d;
	x += dx;
	y += dy;
	dx *= 0.98;
	dy *= 0.98;
	status = 0;
error:
	return status;
}

#define DRAW(...)	SDL_ZERO(draw, __VA_ARGS__)

#define K(scancode)	(kb[SDL_SCANCODE_ ## scancode])
#define B(button)	(buttons & SDL_BUTTON(SDL_BUTTON_ ## button))

void input(void)
{
	const uint8_t *kb;
	int mx, my;
	int footx, footy;
	uint32_t buttons;

	kb = SDL_GetKeyboardState(NULL);
	buttons = SDL_GetMouseState(&mx, &my);

	if (K(LEFT))
		dx -= 0.1;
	if (K(RIGHT))
		dx += 0.1;
	if (K(UP))
		dy -= 0.1;
	if (K(DOWN))
		dy += 0.1;
	if (K(Q))
		tint.r += 1;
	if (K(W))
		tint.g += 1;
	if (K(E))
		tint.b += 1;
	if (K(R))
		tint.a += 1;
	if (K(A))
		tint.r -= 1;
	if (K(S))
		tint.g -= 1;
	if (K(D))
		tint.b -= 1;
	if (K(F))
		tint.a -= 1;

	footx = x + 50;
	footy = y + 200;

	if (B(LEFT)) {
		dx = mx - footx;
		dy = my - footy;
	}
	if (B(RIGHT)) {
		dx = footx - mx;
		dy = footy - my;
	}

	joy_d = hypot(joy_x, joy_y);
	if (joy_d > joy_dead_zone) {
		dx += joy_x / 32768 * 0.1;
		dy += joy_y / 32768 * 0.1;
	} else {
		joy_x = 0;
		joy_y = 0;
		joy_d = 0;
	}
	if (joy_x > 0)
		flip = 0;
	if (joy_x < 0)
		flip = 1;
	if (joy_d)
		joy_a = DEG(atan2(joy_y, joy_x));
	else if (flip)
		joy_a = 180;
	else
		joy_a = 0;
//	INFO("x y a %.2f %.2f %.2f", joy_x, joy_y, joy_a);

	if (!joy_d && dx > 0) {
		flip = 0;
		joy_a = 0;
	} else if (!joy_d && dx < 0) {
		flip = 1;
		joy_a = 180;
	}
}

bool seen_axis[n_joys_max][n_axes_max];

void joy_input(SDL_Event *e)
{
	int joy = e->jaxis.which;
	int axis = e->jaxis.axis;
	int pos = e->jaxis.value;
	if (axis >= n_axes_max) {
		INFO("Joy %d axis %d, axis number too high", joy, axis);
		return;
	}
	if (!seen_axis[joy][axis] && abs(pos) > joy_dead_zone) {
		seen_axis[joy][axis] = 1;
		INFO("Joy %d axis %d", joy, axis);
	}
	if (joy != 0)
		return;
	if (axis == 0)
		joy_x = pos;
	else if (axis == 1)
		joy_y = pos;
}

int main(void)
{
	int exit_status = EXIT_FAILURE;
	bool quit = 0;
	SDL_Event e;

	INIT_SDL();

	back = LOAD_TEXTURE("back.png");
	sprite_sheet = LOAD_TEXTURE("animation.png");

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = 1;
			if (e.type == SDL_JOYAXISMOTION)
				joy_input(&e);
		}
		input();
		DRAW();
	}

	exit_status = EXIT_SUCCESS;
error:
	SDL_DestroyTexture(back);
	SDL_DestroyTexture(sprite_sheet);
	close_sdl();
	exit(exit_status);
}
