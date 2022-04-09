PKGS=sdl2 SDL2_image SDL2_ttf SDL2_mixer
CFLAGS=-std=gnu99 -Wall -Wextra -g -D_FILE_OFFSET_BITS=64 $(shell pkg-config --cflags $(PKGS)) -I../
LDLIBS=-lm $(shell pkg-config --libs $(PKGS))
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),MINGW32_NT-6.1)
	LDLIBS += -lopengl32 -lglu32 -lglew32
else
ifeq ($(UNAME_S),Darwin)
	CFLAGS += -FOpenGL
	LDLIBS += -framework OpenGL
else
	LDLIBS += -lGL -lGLU -lGLEW
endif
endif

EXES=01_hello_sdl 02_getting_an_image_on_the_screen 03_event_driven_programming 04_key_presses 05_optimized_surface_loading_and_soft_stretching 07_texture_loading_and_rendering 08_geometry_rendering 09_the_viewport 10_colour_keying 11_sprite_sheets 12_colour_modulation 13_alpha_blending 14_animated_sprites 16_truetype_fonts 17_mouse_events 18_key_states 19_gamepads_and_joysticks 21_sound_effects_and_music 22_timing 23_advanced_timers 24_calculating_frame_rate 25_capping_frame_rate 26_motion 50_sdl_and_opengl_2 51_sdl_and_modern_opengl

all: $(EXES)

clean:
	rm -f $(EXES)
