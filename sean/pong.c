#include "basic.h"

float x;
float y;

float dx;
float dy;

float b1x = 0 + 40;
float b1y = 300;

float b2x = 800 - 40;
float b2y = 300;

float b1v = 0;
float b2v = 0;

float speed = 3;

float ball_w = 10;
float bat_w = 6;
float bat_h = 60;

float hit_boost = 1.1;

int p1_score = 0;
int p2_score = 0;

void new_ball(void) {
	x = 400;
	y = 300;

	if (random() % 2) {
		dx = 1;
	} else {
		dx = -1;
	}
	dy = (random() % 21 - 10) / 10.0;
}

void init(void)
{
	srandom(time(NULL));
	new_ball();
}

void draw(void)
{
//	colour(255, 255, 255);

	printf("\r%d %d ", p1_score, p2_score);
	fflush(stdout);
	
	//Ball
	rectc(x, y, ball_w, ball_w);
	
	//Net
	rect(399, 0, 2, 600);
	
	//Player 1 bat
	rectc(b1x, b1y, bat_w, bat_h);

	//Player 2 bat
	rectc(b2x, b2y, bat_w, bat_h);

	//ball movement
	x += dx;
	y += dy;

	if (y < 0) {
		dy = -dy;
		y = -y;
	}
	if (y > 600) {
		dy = -dy;
		y = 2*600 - y;
	}

	//player 1 bat movement
	b1y = b1y + b1v;

	if (b1y < bat_h / 2) {
		b1y = bat_h / 2;
	}

	if (b1y > 600 - bat_h / 2) {
		b1y = 600 - bat_h / 2;
	}

	//player 2 bat movement
	b2y = b2y + b2v;
	
	if (b2y < bat_h / 2) {
		b2y = bat_h / 2;
	}

	if (b2y > 600 - bat_h / 2) {
		b2y = 600 - bat_h / 2;
	}

	// player 1 hit the ball

	// 1. check x + ball_w/2 past the bat - bat_w/2 and previous x was not past the bat
	// 2. check y between b1y - bat_h/2 and b1y + bat_h/2

	float xo = x - dx;

	if (x - ball_w/2 <= b1x + bat_w/2  &&  xo - ball_w/2 > b1x + bat_w/2  &&  y + ball_w/2 >= b1y - bat_h/2  &&  y - ball_w/2 <= b1y + bat_h/2) {
		dx = -dx * hit_boost;
		dy = (random() % 21 - 10) / 10.0;
	}

	// player 2 hit the ball
	if (x + ball_w/2 >= b2x - bat_w/2  &&  xo + ball_w/2 < b2x - bat_w/2  &&  y + ball_w/2 >= b2y - bat_h/2  &&  y - ball_w/2 <= b2y + bat_h/2) {
		dx = -dx * hit_boost;
		dy = (random() % 21 - 10) / 10.0;
	}

	// check for player 1 win
	if (x - ball_w/2 >= 800) {
		p1_score++;
		new_ball();
	}

	// check for player 2 win
	if (x + ball_w/2 < 0) {
		p2_score++;
		new_ball();
	}
}

// TODO make simpler key check
// TODO random in range for int and float

void keydown(int key)
{
	if (key == SDLK_w) {
		b1v -= speed;
	} else if (key == SDLK_s) {
		b1v += speed;
	} else if (key == SDLK_UP) {
		b2v -= speed;
	} else if (key == SDLK_DOWN) {
		b2v += speed;
	}
}

void keyup(int key)
{
	if (key == SDLK_w) {
		b1v += speed;
	} else if (key == SDLK_s) {
		b1v -= speed;
	} else if (key == SDLK_UP) {
		b2v += speed;
	} else if (key == SDLK_DOWN) {
		b2v -= speed;
	}
}
