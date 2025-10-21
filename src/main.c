#include "raylib.h"
#include "stdio.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir


void action(KeyboardKey x){

}

// define a timer
typedef struct
{
    float Lifetime;
}Timer;

// start or restart a timer with a specific lifetime
void StartTimer(Timer* timer, float lifetime)
{
    if (timer != NULL)
        timer->Lifetime = lifetime;
}

// update a timer with the current frame time
void UpdateTimer(Timer* timer)
{
    // subtract this frame from the timer if it's not allready expired
    if (timer != NULL && timer->Lifetime > 0)
        timer->Lifetime -= GetFrameTime();
}

// check if a timer is done.
bool TimerDone(Timer* timer)
{
    if (timer != NULL)
        return timer->Lifetime <= 0;

	return false;
}



int main ()
{
	


	int playerspeed = 5;
	int posx=64;
	int posy=640;
	int dir = 1;
	
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");
	SetTargetFPS(60);

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture playersprite1 = LoadTexture("spritepfront.png");
	Texture playersprite3 = LoadTexture("spritepback.png");
	Texture playersprite2 = LoadTexture("spritepsideright.png");
	Texture playersprite4 = LoadTexture("spritepsideleft.png");
	Texture note1 = LoadTexture("do.png");
	Texture note2 = LoadTexture("re.png");
	Texture note3 = LoadTexture("mi.png");
	Texture note4 = LoadTexture("fa.png");
	Texture note5 = LoadTexture("so.png");
	Texture note6 = LoadTexture("la.png");
	Texture note7 = LoadTexture("si.png");

	Timer turntimer = {0};
	float turnduration = 3.0f;
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{

		
		if (TimerDone(&turntimer)){
			if(IsKeyPressed(KEY_DOWN)) {
				posy += 64;
				dir = 1;
				StartTimer(&turntimer,turnduration);
			}
			if(IsKeyPressed(KEY_UP)) {
				posy -= 64;
				dir = 3;
				StartTimer(&turntimer,turnduration);
			}
			if(IsKeyPressed(KEY_RIGHT)) {
				posx += 64;
				dir = 2;
				StartTimer(&turntimer,turnduration);
			}
			if(IsKeyPressed(KEY_LEFT)) {
				posx -= 64;
				dir = 4;
				StartTimer(&turntimer,turnduration);
			}
		}

		UpdateTimer(&turntimer);

		int pcolx = posx+32; 
		int pcoly = posy+64;
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		DrawText("Hello Raylib ", 200,200,20,WHITE);

		// draw our texture to the screen
		DrawCircle(pcolx,pcoly,1, GREEN);
		if (dir == 1) DrawTexture(playersprite1, posx, posy, WHITE);
		if (dir == 3) DrawTexture(playersprite3, posx, posy, WHITE);
		if (dir == 2) DrawTexture(playersprite2, posx, posy, WHITE);
		if (dir == 4) DrawTexture(playersprite4, posx, posy, WHITE);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(playersprite1);
	UnloadTexture(playersprite2);
	UnloadTexture(playersprite3);
	UnloadTexture(playersprite4);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
