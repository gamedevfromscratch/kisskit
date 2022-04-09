#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include "kiss.h"

SDL_Surface *surface;
Uint32 rmask, gmask, bmask, amask;
uint32_t colour = 0;

int init_sdl(int width, int height)
{
	int status = -1;

	SDL_INIT(0);
	IMG_INIT(IMG_INIT_PNG);

	if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	} else {
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	}

	surface = SDL_CREATERGBSURFACE(0, width, height, 32, rmask, gmask, bmask, amask);
	status = 0;
error:
	return status;
}

#define INIT_SDL(...) SDL_ZERO(init_sdl, __VA_ARGS__)

void close_sdl(void)
{
	SDL_FreeSurface(surface);
	SDL_Quit();
}

#define deg2rad(a) ((a) * M_PI / 180.0)
#define rad2deg(a) ((a) * 180.0 / M_PI)

#define rdiv(r, base) (floor((r) / (base)))

double rmod(double r, double base)
{
	int d = rdiv(r, base);
	return r - d * base;
}

int iclamp(int x, int min, int max)
{
	return x < min ? min : x > max ? max : x;
}

uint32_t rgb(double red, double green, double blue)
{
	int r = iclamp(red * 256, 0, 255);
	int g = iclamp(green * 256, 0, 255);
	int b = iclamp(blue * 256, 0, 255);
	colour = SDL_MapRGB(surface->format, r, g, b);
	return colour;
}

double rb_red_angle, rb_green_angle, rb_blue_angle, rb_red_power, rb_green_power, rb_blue_power;

uint32_t rb[360];

#define rainbow(a) (_rainbow(deg2rad(a)))
uint32_t _rainbow(double a)
{
	double r = rb_red_power * (cos(a - rb_red_angle) + 1) / 2;
	double g = rb_green_power * (cos(a - rb_green_angle) + 1) / 2;
	double b = rb_blue_power * (cos(a - rb_blue_angle) + 1) / 2;
	return rgb(r, g, b);
}

void rainbow_init(void)
{
	rb_red_angle = deg2rad(-120);
	rb_green_angle = 0;
	rb_blue_angle = deg2rad(120);

	rb_red_power = 1;
//      rb_green_power = 0.8;
	rb_green_power = 0.9;
	rb_blue_power = 1;

	for (int i = 0; i < 360; ++i)
		rb[i] = _rainbow(deg2rad(i));
}

void rect_fill(int x, int y, int w, int h)
{
	SDL_FILLRECT(surface, &(SDL_Rect) {
		     x, y, w, h}, colour);
error:
	return;
}

void sierpinski(int x, int y, int r, float c)
{
	rainbow(c * 360 / 12.0);
	rect_fill(x, y, r, r);
	if (r < 3)
		return;
	r /= 3;
	float c1, c2, c3;
	c1 = c + 5;
	c2 = c + 7;
	c3 = c + 1;
	c1 = rmod(c1, 12);
	c2 = rmod(c2, 12);
	c3 = rmod(c3, 12);
	sierpinski(x, y, r, c1);
	sierpinski(x + r, y, r, c2);
	sierpinski(x + r * 2, y, r, c1);
	sierpinski(x + r * 2, y + r, r, c2);
	sierpinski(x + r * 2, y + r * 2, r, c1);
	sierpinski(x + r, y + r * 2, r, c2);
	sierpinski(x, y + r * 2, r, c1);
	sierpinski(x, y + r, r, c2);
	sierpinski(x + r, y + r, r, c3);
}

int main(void)
{
	int exit_status = EXIT_FAILURE;
	int size = pow(3, 7);

	INIT_SDL(size, size);
	rainbow_init();
	sierpinski(0, 0, size, 2);
	IMG_SAVEPNG(surface, "carpet6.png");

	exit_status = EXIT_SUCCESS;
error:
	close_sdl();
	exit(exit_status);
}
