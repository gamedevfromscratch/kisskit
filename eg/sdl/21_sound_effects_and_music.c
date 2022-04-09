#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "kiss.h"

char *title = "SDL 21 - Sound Effects and Music";
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *back;

Mix_Music *music;
Mix_Chunk *effect;

int init_sdl(void)
{
	int status = -1;
	int x = SDL_WINDOWPOS_UNDEFINED;
	int y = SDL_WINDOWPOS_UNDEFINED;
	int width = 800;
	int height = 600;
	int flags = 0;		// SDL_WINDOW_FULLSCREEN_DESKTOP
	int sample_rate = 44100;
	int channels = 2;
	int chunk_size = 2048;

	SDL_INIT(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_INIT(IMG_INIT_PNG);
	MIX_INIT(MIX_INIT_OGG);
	MIX_OPENAUDIO(sample_rate, MIX_DEFAULT_FORMAT, channels, chunk_size);
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

void close_sdl(void)
{
	SDL_DESTROYRENDERER(renderer);
	SDL_DESTROYWINDOW(window);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

int draw(void)
{
	int status = -1;
	SDL_RENDERCOPY(renderer, back, NULL, NULL);
	SDL_RenderPresent(renderer);
	status = 0;
error:
	return status;
}

#define DRAW(...) SDL_ZERO(draw, __VA_ARGS__)

int toggle_music(void)
{
	int status = -1;

	if (!Mix_PlayingMusic())
		MIX_PLAYMUSIC(music, 1);
	else if (Mix_PausedMusic())
		Mix_ResumeMusic();
	else
		Mix_PauseMusic();
	status = 0;
error:
	return status;
}

#define TOGGLE_MUSIC(...) SDL_ZERO(toggle_music, __VA_ARGS__)

int key_down(SDL_Keycode k)
{
	int status = -1;
	switch (k) {
	case SDLK_1:
		MIX_PLAYCHANNEL(-1, effect, 0);
		break;

	case SDLK_9:
		TOGGLE_MUSIC();
		break;

	case SDLK_0:
		Mix_HaltMusic();
		break;
	}
	status = 0;
error:
	return status;
}

#define KEY_DOWN(...) SDL_ZERO(key_down, __VA_ARGS__)

int main(void)
{
	int exit_status = EXIT_FAILURE;
	bool quit = 0;
	SDL_Event e;

	INIT_SDL();

	back = LOAD_TEXTURE("back.png");
	music = MIX_LOADMUS("purple_motions.xm");
	effect = MIX_LOADWAV("clarinet.ogg");

	while (!quit) {
		DRAW();
		SDL_WAITEVENT(&e);
		if (e.type == SDL_QUIT)
			quit = 1;
		else if (e.type == SDL_KEYDOWN)
			KEY_DOWN(e.key.keysym.sym);
	}

	exit_status = EXIT_SUCCESS;
error:
	MIX_FREECHUNK(effect);
	MIX_FREEMUSIC(music);
	SDL_DESTROYTEXTURE(back);
	close_sdl();
	exit(exit_status);
}
