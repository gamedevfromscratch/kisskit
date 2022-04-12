#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include "kiss.h"

char *title = "SDL 50 - SDL and OpenGL 2";
SDL_Window *window;
SDL_GLContext gl_context;
bool draw_quad = 1;

GLuint program = 0;
GLint vertex_pos_2d_loc = -1;
GLuint VBO = 0;
GLuint IBO = 0;

void print_program_log(GLuint name)
{
	int maxlen, len;
	CHECK1(glIsProgram(name), "GL Name %d is not a program", name);
	glGetProgramiv(name, GL_INFO_LOG_LENGTH, &maxlen);
	INFO("GL_INFO_LOG_LENGTH = %d", maxlen);
	{
		char log[maxlen];
		glGetProgramInfoLog(name, maxlen, &len, log);
		if (len > 0)
			EPRINT("%s\n", log);
	}
error:
	return;
}

void print_shader_log(GLuint name)
{
	int maxlen, len;
	CHECK1(glIsShader(name), "GL Name %d is not a shader", name);
	glGetShaderiv(name, GL_INFO_LOG_LENGTH, &maxlen);
	INFO("GL_INFO_LOG_LENGTH = %d", maxlen);
	{
		char log[maxlen];
		glGetShaderInfoLog(name, maxlen, &len, log);
		if (len > 0)
			EPRINT("%s\n", log);
	}
error:
	return;
}

int init_sdlgl(void)
{
	int status = -1;
	GLint success;

	SDL_INIT(SDL_INIT_VIDEO);
	SDL_GL_SETATTRIBUTE(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SETATTRIBUTE(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CREATEWINDOW(title, 50, 50, 800, 600, SDL_WINDOW_OPENGL);
	gl_context = SDL_GL_CREATECONTEXT(window);
	glewExperimental = GL_TRUE;
	GLEWINIT();
	SDL_GL_SetSwapInterval(1);

	program = glCreateProgram();

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *vertex_shader_src[] = {
		"#version 140\n"
		"in vec2 LVertexPos2D;\n"
		"void main() {\n"
		"	gl_Position = vec4(LVertexPos2D.x, LVertexPos2D.y, 0, 1);\n"
		"}\n"
	};
	glShaderSource(vertex_shader, 1, vertex_shader_src, NULL);
	glCompileShader(vertex_shader);
	success = GL_FALSE;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		print_shader_log(vertex_shader);
		CHECK1(0, "glCompileShader failed: vertex_shader");
	}
	glAttachShader(program, vertex_shader);

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar *fragment_shader_src[] = {
		"#version 140\n"
		"out vec4 LFragment;\n"
		"void main() {\n"
		"	LFragment = vec4(1.0, 0.8, 0.6, 1.0);\n"
		"}"
	};
	glShaderSource(fragment_shader, 1, fragment_shader_src, NULL);
	glCompileShader(fragment_shader);
	success = GL_FALSE;
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		print_shader_log(fragment_shader);
		CHECK1(0, "glCompileShader failed: fragment_shader");
	}
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success != GL_TRUE) {
		print_program_log(program);
		CHECK1(0, "glLinkProgram failed");
	}

	vertex_pos_2d_loc = glGetAttribLocation(program, "LVertexPos2D");
	CHECK1(vertex_pos_2d_loc != -1, "glGetAttribLocation failed: LVertexPos2D");

	glClearColor(0.5f, 0.f, 0.f, 1.f);

	GLfloat vertex_data[] = {
		0, -.5,
		.5, 0,
		0, .5,
		-.5, 0
	};

	GLuint index_data[] = { 0, 1, 2, 3 };

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertex_data, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), index_data, GL_STATIC_DRAW);

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

	glUseProgram(program);
	glEnableVertexAttribArray(vertex_pos_2d_loc);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(vertex_pos_2d_loc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
	glDisableVertexAttribArray(vertex_pos_2d_loc);
	glUseProgram(0);
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
