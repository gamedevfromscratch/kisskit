#include "basic.h"

void colour(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	// error?
}

void rect(int x, int y, int w, int h)
{
	SDL_Rect clip = { x, y, w, h };
	SDL_RENDERFILLRECT(renderer, &clip);
error:
}

void circle(int x, int y, int radius)
{
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	filledCircleRGBA(renderer, x, y, radius, r, g, b, a);
	// wrapper error handle
}
