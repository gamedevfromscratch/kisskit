#include "basic.h"

int y = 0;
int dy = 0;

void draw(void)
{
	colour(30, 60, 90);
	rect(100, y, 200, 300);
	y += dy;
}

void keydown(int key)
{
	if (key == SDLK_UP) {
		printf("Up\n");
		dy--;
	} else if (key == SDLK_DOWN) {
		printf("Down\n");
		dy++;
	}
}
