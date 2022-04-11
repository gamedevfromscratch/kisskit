PKGS=sdl2 SDL2_image SDL2_ttf SDL2_mixer SDL2_gfx
CFLAGS=-Wall -Wextra -Wstrict-prototypes -g -D_FILE_OFFSET_BITS=64 -Uunix -Ulinux $(shell pkg-config --cflags $(PKGS)) -I.
LDLIBS=basic/basic.a -lm $(shell pkg-config --libs $(PKGS))
UNAME_S := $(shell uname -s)
MACHINE := $(shell gcc -dumpmachine)
CC_BASENAME := $(shell basename $(CC))

ifeq ($(OS),Windows_NT)
	LDLIBS += -lopengl32 -lglu32 -lglew32
else
ifeq ($(UNAME_S),Darwin)
	CFLAGS += -FOpenGL
	LDLIBS += -framework OpenGL
else
	LDLIBS += -lGL -lGLU -lGLEW
endif
endif

ifeq ($(CC_BASENAME),tcc)
	CFLAGS+=-DSDL_DISABLE_IMMINTRIN_H
ifeq ($(UNAME_S),Linux)
	LDLIBS += -L/usr/lib/$(MACHINE)/pulseaudio
endif
endif

default:

build-dep:
	sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev libglew-dev libsdl2-gfx-dev

basic/basic.a: basic/basic.o basic/init.o basic/drawing.o basic/draw.o basic/keydown.o basic/keyup.o basic/title.o
	ar r $@ $^
