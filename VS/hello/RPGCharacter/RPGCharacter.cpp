#include <iostream>
#include <raylib.h>

using namespace std;

const int screen_width = 800;
const int screen_height = 800;
const int sprite_size = 320;
const float player_speed = 100.0f;
const float frame_delay = 0.2f;

int main()
{
	cout << "Starting RPG Game." << endl;

	const char* title = "RPG Game";

	InitWindow(screen_width, screen_height, title);
	SetTargetFPS(60);

	Texture2D spriteSheet = LoadTexture("assets/Girl-Sprite-0003.png");

	Vector2 playerPos = { screen_width / 2.0f, screen_height / 2.0f };
	int frame = 0;
	int frameTime = 0;

	int animationRow = 1;
	bool isFacingLeft = false;

	while (WindowShouldClose() == false) {

		float deltaTime = GetFrameTime();
		frameTime += deltaTime;

		Vector2 dir = { 0, 0 };
		if (IsKeyDown(KEY_UP)) {
			dir.y = -1;
			animationRow = 2;
		}
		else if (IsKeyDown(KEY_DOWN)) {
			dir.y = 1;
			animationRow = 0;
		}
		else if (IsKeyDown(KEY_LEFT)) {
			dir.x = -1;
			animationRow = 3;
			isFacingLeft = true;
		}
		else if (IsKeyDown(KEY_RIGHT)) {
			dir.x = 1;
			animationRow = 3;
			isFacingLeft = false;
		}
		else {
			animationRow = 1;
		}

		if (dir.x != 0 || dir.y != 0) {
			float length = sqrtf(dir.x * dir.x + dir.y * dir.y);

			dir.x /= length;
			dir.y /= length;

			playerPos.x += dir.x * player_speed * deltaTime;
			playerPos.y += dir.y * player_speed * deltaTime;

			if (frameTime >= frame_delay) {
				frame = (frame + 1) % 4;
				frameTime = 0;
			}
		}
		else {
			frame = 0;
		}

		BeginDrawing();
		ClearBackground(DARKGREEN);
		
		Rectangle sourceRec = {
			(float)(frame * sprite_size),
			(float)(animationRow * sprite_size),
			isFacingLeft ? -sprite_size : sprite_size,
			(float)sprite_size
		};

		Vector2 origin = { 0, 0 };
		Vector2 pos = { playerPos.x, playerPos.y };

		DrawTextureRec(spriteSheet, sourceRec, pos, WHITE);

		EndDrawing();
	}

	UnloadTexture(spriteSheet);
	CloseWindow();

	return 0;
}

