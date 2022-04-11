#include "basic/deps.h"

extern SDL_Window *window;
extern SDL_Renderer *renderer;

void init(void);
void draw(void);
void keydown(int key);
void keyup(int key);
extern int f;
extern char *title;

void colour(Uint8 r, Uint8 g, Uint8 b);
#define color(r, g, b) colour(r, g, b)
void rect(int x, int y, int w, int h);
void circle(int x, int y, int r);
