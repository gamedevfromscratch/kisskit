#include "basic.h"

void colour(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	// error?
}

void line(int x0, int y0, int x1, int y1)
{
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	lineRGBA(renderer, x0, y0, x1, y1, r, g, b, a);
}

void rect(int x, int y, int w, int h)
{
	SDL_Rect clip = { x, y, w, h };
	SDL_RENDERFILLRECT(renderer, &clip);
error:
	;
}

void rectc(int cx, int cy, int w, int h)
{
	// TODO better use x and y "radius"?
	rect(cx - w / 2, cy - h / 2, w, h);
}

void circle(int x, int y, int radius)
{
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	filledCircleRGBA(renderer, x, y, radius, r, g, b, a);
	// TODO handle errors
}

void circle0(int x, int y, int radius)
{
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	circleRGBA(renderer, x, y, radius, r, g, b, a);
	// TODO handle errors
}
