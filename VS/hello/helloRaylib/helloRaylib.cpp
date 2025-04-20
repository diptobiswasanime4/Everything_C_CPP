#include <iostream>
#include <raylib.h>

using namespace std;

struct Ball {
	int x;
	int y;
	int radius;
	Color color;
	int velX;
	int velY;
};

int main()
{
	cout << "Starting the Game." << endl;

	const int screen_width = 1280;
	const int screen_height = 800;
	const char* title = "Pong Pong";
	SetTargetFPS(60);

	Ball ball = {
		screen_width / 2,
		screen_height / 2,
		20,
		GREEN,
		3,
		3
	};

	InitWindow(screen_width, screen_height, title);

	while (WindowShouldClose() == false) {
		ClearBackground(BLACK);
		BeginDrawing();

		DrawCircle(ball.x, ball.y, ball.radius, ball.color);

		if (ball.x < 0 || ball.x > screen_width) {
			ball.velX = -ball.velX;
		}
		if (ball.y < 0 || ball.y > screen_height) {
			ball.velY = -ball.velY;
		}

		ball.x += ball.velX;
		ball.y += ball.velY;

		EndDrawing();
	}

	CloseWindow();

	return 0;
}

