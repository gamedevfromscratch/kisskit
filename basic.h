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
void line(int x0, int y0, int x1, int y1);
void rect(int x, int y, int w, int h);
void rectc(int cx, int cy, int w, int h);
void circle(int x, int y, int r);
void circle0(int x, int y, int r);
void tri(int x0, int y0, int x1, int y1, int x2, int y2);
void tri0(int x0, int y0, int x1, int y1, int x2, int y2);
