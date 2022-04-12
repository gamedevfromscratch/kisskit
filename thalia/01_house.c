#include "basic.h"

void draw(void)
{
	colour(0, 100, 250);
	rect(0, 0, 800, 500);

	colour(0, 250, 100);
	rect(0, 500, 800, 100);

	colour(200, 50, 30);
	rect(200, 300, 400, 300);

	colour(250, 120, 100);
	rect(340, 400, 120, 200);

	colour(80, 150, 200);
	circle(270, 400, 50);
	circle(530, 400, 50);
}
