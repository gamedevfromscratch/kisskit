# Changelog

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/) but in ascending order.
This project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [0.0.0] - 2015-11-01

### Added

- initial README

## [0.0.1] - 2022-04-09

### Added

- utility macros
- logging and error macros, NDEBUG option
- TODO, FIXME, XXX macros
- error handling wrapper macros for libc, SDL, GL, UNIX; not comprehensive
- Use GNU C extension: [statement expressions](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html).
- basic library to enable writing very simple graphical programs
- support for Linux, Windows and Mac OS
- `kr` script to run `indent` and reindent code in K&R style; it's not 100%
- example programs
- SDL tutorial programs
- HTML page to compare code with and without kiss.h

## Unreleased - 2022-04-12

### Added

- basic:
	- colour function
	- line function
	- example: eg/sdl_gfx.c
- autorun script
	- Rebuild and re-run a program when the source code is saved.
	- conf/vimrc remaps <F1> to save the file
	- When we change a drawing program, we can save and see the changes
	  right away.

### Fixed:

- default colour, white
- follow an `error:` label at the end with a semi-colon, for gcc-10
- move some files and folders around

### Work in Progress:

- basic:
	- rectc function, to draw a rectangle from its center
	- text drawing with TTF
	- random numbers
	- circle0 function
	- tri and tri0 functions
	- line_width

- brace and unbrace tools, to program in C with a light syntax like Python.

### To-do:

- use single function for fill and outline, with separate colour options
- use line_width for all shapes
- use GL instead of SDL gfx?
- automatic header generation
