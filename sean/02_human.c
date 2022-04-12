#include "basic.h"

char *title = "Human";

void draw(void)
{
	int x = 400 + f;
	int y = 300;

	colour(255, 127, 63);

	rect(x - 100, y, 200, 300);

	rect(x - 100, y + 300, 75, 300);
	rect(x + 25, y + 300, 75, 300);

	rect(x - 250, y, 150, 75);
	rect(x + 100, y, 150, 75);

	colour(0, 0, 255);
	circle(x, y - 75, 150);
}
