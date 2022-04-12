
	if (x - ball_w/2 <= b1x + bat_w/2) {
		INFO("ball is left of P1");
	}
	if (xo - ball_w/2 > b1x + bat_w/2) {
		INFO("ball WAS right of P1");
	}
	if (y + ball_w/2 >= b1y - bat_h/2) {
		INFO("ball is not above P1");	      
	}
	if (y - ball_w/2 <= b1y + bat_h/2) {
		INFO("ball is not below P1");
	}



TTF_Font *font;
font = TTF_OPENFONT("OpenSans-Regular.ttf", 112);

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


void draw_score(int score, float x, float y)
{
	// text1 = TEXTURE_FROM_TEXT(font, "The quick brown fox jumps over the lazy dog", (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF});
	UNUSED(score);
	UNUSED(x);
	UNUSED(y);
}

	draw_score(p1_score, 300, 50);
	draw_score(p2_score, 500, 50);
