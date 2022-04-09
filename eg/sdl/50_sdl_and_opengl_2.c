#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include "kiss.h"

char *title = "SDL 50 - SDL and OpenGL 2";
SDL_Window *window;
SDL_GLContext gl_context;
bool draw_quad = 1;

int init_sdlgl(void)
{
	int status = -1;

	SDL_INIT(SDL_INIT_VIDEO);
	SDL_GL_SETATTRIBUTE(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SETATTRIBUTE(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	window = SDL_CREATEWINDOW(title, 50, 50, 800, 600, SDL_WINDOW_OPENGL);
	gl_context = SDL_GL_CREATECONTEXT(window);
	SDL_GL_SetSwapInterval(1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GL_CHECK("GL init 1");

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GL_CHECK("GL init 2");

	glClearColor(0, 0, 1, 1);

	GL_CHECK("GL init 3");

	// TODO On OS X make sure you bind 0 to the draw framebuffer before swapping the window, otherwise nothing will happen. ???

	status = 0;
error:
	return status;
}

#define INIT_SDLGL(...) SDL_ZERO(init_sdlgl, __VA_ARGS__)

void close_sdl(void)
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (!draw_quad)
		return;

	glColor4f(0, 1, 0.5, 1);
	glBegin(GL_QUADS);
	glVertex2f(-.5, -.6);
	glVertex2f(.6, -.5);
	glVertex2f(.5, .6);
	glVertex2f(-.6, .5);
	glEnd();
}

void key_down(SDL_Keycode k)
{
	switch (k) {
	case SDLK_SPACE:
		draw_quad = !draw_quad;
		break;
	}
}

int main(void)
{
	int exit_status = EXIT_FAILURE;
	bool quit = 0;
	SDL_Event e;

	INIT_SDLGL();

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = 1;
			if (e.type == SDL_KEYDOWN)
				key_down(e.key.keysym.sym);
		}

		draw();
		SDL_GL_SwapWindow(window);
		GL_CHECK("GL Frame");
	}

	exit_status = EXIT_SUCCESS;
error:
	close_sdl();
	exit(exit_status);
}
