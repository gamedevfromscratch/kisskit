#include "basic.h"

extern char *title;
SDL_Window *window;
SDL_Renderer *renderer;
//SDL_Surface *screen;
// SDL_Texture *obj_texture;

int f = 0;

int init_sdl(void)
{
	int status = -1;
	int sample_rate = 44100;
	int channels = 2;
	int chunk_size = 2048;

	SDL_INIT(SDL_INIT_VIDEO);
	IMG_INIT(IMG_INIT_PNG);
	MIX_INIT(MIX_INIT_OGG);
	MIX_OPENAUDIO(sample_rate, MIX_DEFAULT_FORMAT, channels, chunk_size);
	TTF_INIT();
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
	TTF_Quit();
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

int main(int argc, char *argv[])
{
	int exit_status = EXIT_FAILURE;
	SDL_Event e;

	if (title)
		;
	else if (argc)
		title = argv[0];
	else
		title = "Basic SDL App";

	INIT_SDL();

	init();

	for (;;) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
				goto quit;
			else if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
				keydown(e.key.keysym.sym);
			else if (e.type == SDL_KEYUP)
				keyup(e.key.keysym.sym);
		}

		colour(0, 0, 0);
		SDL_RENDERCLEAR(renderer);
		colour(255, 255, 255);
		draw();
		SDL_RenderPresent(renderer);

		f++;
	}

quit:
	exit_status = EXIT_SUCCESS;

error:
//	SDL_DestroyTexture(obj_texture);
	close_sdl();
	exit(exit_status);
}
