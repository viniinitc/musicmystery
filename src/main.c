#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

typedef struct{

	int first;
	int last;
	int cur;

	float speed;
	float durationleft;

}animation;




typedef struct{

	int typenote;
	int posny;
	int posnx;
	int direct;
	int turnr;
	struct notes* next;

}notes;


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

void shootmove(notes* x){

	for(int i = 0; i < 7; i++){
		
		int mov = x[i].direct;
			
		if (mov == 1){
			x[i].posny += 64;
		}
		if (mov == 3){
			x[i].posny -= 64;
		}
		if (mov == 2){
			x[i].posnx += 64;
		}
		if (mov == 4){
			x[i].posnx -= 64;
		}
		
	}

}

typedef struct{

	int hp;
	int type;
	struct enemytype* next;

}enemytype;



int main ()
{
	notes notelist[7];
	
	int posx=64;
	int posy=640;
	int firstblockx = 48;
	int firstblocky = 708;
	int dir = 1;
	int playerhp = 3;
	int turn = 0;
	int turncom;
	int notecheck = 0;
	int cont = 0;

	bool collision = false;

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 900, "Hello Raylib");
	SetTargetFPS(60);

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	
	Texture playersprite3 = LoadTexture("spritepback.png");
	Texture playersprite4 = LoadTexture("spritepsideleft.png");
	Texture notesprite1 = LoadTexture("do.png");
	Texture notesprite2 = LoadTexture("re.png");
	Texture notesprite3 = LoadTexture("mi.png");
	Texture notesprite4 = LoadTexture("fa.png");
	Texture notesprite5 = LoadTexture("sol.png");
	Texture notesprite6 = LoadTexture("la.png");
	Texture notesprite7 = LoadTexture("si.png");
	Texture2D floortest1 = LoadTexture("floor1.png");
	Texture2D playersprite1 = LoadTexture("playeranimation1.png");
	Texture2D playersprite2 = LoadTexture("playeranimation2.png");
	
	Timer turntimer = {0};

	float turnduration = 1.0f;
	float animationdur = 5.0f;

	Vector2 position = { 350.0f, 280.0f};
	Vector2 positionplayer = { 40.0f, 272.0f};
	Rectangle frameRec = {0.0f, 0.0f, (float)floortest1.width/16, (float)floortest1.height};
	Rectangle frameRecplayer = {0.0f, 0.0f, (float)playersprite1.width/16, (float)playersprite1.height};
	Rectangle frameRecplayer2 = {0.0f, 0.0f, (float)playersprite2.width/16, (float)playersprite2.height};

	int currentFrame = 0;
	int framesCounter = 0;
	int framesSpeed = 12;

	for (int i = 0; i < 7; i++){
		notelist[i].typenote = i+1;
	}


	int position1x;
	int position1y;
	int position2x;
	int position2y;
	int position3x;
	int position3y;
	int position4x;
	int position4y;

	

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{

		//remenber to create more collision checks for the remaining balls
		Vector2 contact = {(float)position1x, (float)position1y};
		collision = CheckCollisionPointCircle(GetMousePosition(),contact, 20.0);

		
		framesCounter++;

		if (framesCounter >= (60/framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;

            if (currentFrame > 15) currentFrame = 0;

            frameRec.x = (float)currentFrame*(float)floortest1.width/16;
			frameRecplayer.x = (float)currentFrame*(float)playersprite1.width/16;
			frameRecplayer2.x = (float)currentFrame*(float)playersprite1.width/16;
        }
		
		//remember to update player movement 
		if (TimerDone(&turntimer)){
			if(IsKeyPressed(KEY_DOWN)) {
				posy += 96;
				dir = 1;
				turn++;
				StartTimer(&turntimer,turnduration);
				turncom = turn;
				shootmove(notelist);
			}
			if(IsKeyPressed(KEY_UP)) {
				posy -= 96;
				dir = 3;
				turn++;
				StartTimer(&turntimer,turnduration);
				turncom = turn;
				shootmove(notelist);
			}
			if(IsKeyPressed(KEY_RIGHT)) {
				posx += 96;
				dir = 2;
				turn++;
				StartTimer(&turntimer,turnduration);
				turncom = turn;
				shootmove(notelist);
			}
			if(IsKeyPressed(KEY_LEFT)) {
				posx -= 96;
				dir = 4;
				turn++;
				StartTimer(&turntimer,turnduration);
				turncom = turn;
				shootmove(notelist);
			}
			
			if(IsKeyPressed(KEY_SPACE) && turn != 0){
				turn++;
				
				
				
				notelist[notecheck].direct = dir;
				notelist[notecheck].posnx = posx;
				notelist[notecheck].posny = posy;
				if(dir == 1) notelist[notecheck].posny += 64;
				if(dir == 3) notelist[notecheck].posny -= 64;
				if(dir == 2) notelist[notecheck].posnx += 64;
				if(dir == 4) notelist[notecheck].posnx -= 64;
				notelist[notecheck].turnr = turn;
				
				//shoot(n);
				//n = n->next;
				StartTimer(&turntimer,turnduration);
				shootmove(notelist); //n = n->next; removed this for now so i can figure out the movement of the individual notes
				turncom = turn;

			}
		}
		if(IsKeyPressed(KEY_ONE)) {
				notecheck++;
				if (notecheck > 6) notecheck = 0;
		}

		if (IsKeyPressed(KEY_CAPS_LOCK)) {
			
			//movement balls

			position1x = (int)positionplayer.x + 154;
			position1y = (int)positionplayer.y + 65; 

			position2x = (int)positionplayer.x;
			position2y = (int)positionplayer.y + 45; 

			position3x = (int)positionplayer.x + 144;
			position3y = (int)positionplayer.y + 170; 

			position4x = (int)positionplayer.x;
			position4y = (int)positionplayer.y + 160; 
			

			
		}
		


		UpdateTimer(&turntimer);

		int pcolx = posx+32; 
		int pcoly = posy+64;
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(WHITE);

		// draw some text using the default font
		DrawText("Hello Raylib ", 200,200,20,WHITE);

		// draw our texture to the screen
		

		
		//create if statements so things only get drawn when i need them
		
		DrawTexture(floortest1, 20, 40, WHITE);
		if (dir == 1) DrawTextureRec(playersprite1, frameRecplayer, positionplayer, WHITE);
		if (dir == 3) DrawTextureRec(playersprite2, frameRecplayer2, positionplayer, WHITE);
		DrawTexture(notesprite1, notelist[0].posnx, notelist[0].posny, WHITE); //draw texture also to figure out the individual note movement
		DrawTexture(notesprite2, notelist[1].posnx, notelist[1].posny, WHITE);
		DrawTexture(notesprite3, notelist[2].posnx, notelist[2].posny, WHITE);
		DrawTexture(notesprite4, notelist[3].posnx, notelist[3].posny, WHITE);
		DrawTexture(notesprite5, notelist[4].posnx, notelist[4].posny, WHITE);
		DrawTexture(notesprite6, notelist[5].posnx, notelist[5].posny, WHITE);
		DrawTexture(notesprite7, notelist[6].posnx, notelist[6].posny, WHITE);

		//temporary mouse text so i can figure out positions
		DrawTextEx(GetFontDefault(), TextFormat("[%i, %i]", GetMouseX(), GetMouseY()),
                Vector2Add(GetMousePosition(), (Vector2){ -44, -24 }), 20, 2, BLACK);

		if (collision) DrawText("come on", 100, 100, 100, BLACK);

		DrawCircle(position1x ,position1y, 20, GRAY);

		DrawCircle(position2x ,position2y , 20, GRAY);

		DrawCircle(position3x,position3y, 20, GRAY);

		DrawCircle(position4x ,position4y , 20, GRAY);
		

		//att player sprites
		if (dir == 2) DrawTexture(playersprite2, posx, posy, WHITE);
		if (dir == 4) DrawTexture(playersprite4, posx, posy, WHITE);

		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(notesprite1);
	UnloadTexture(notesprite2);
	UnloadTexture(notesprite3);
	UnloadTexture(notesprite4);
	UnloadTexture(notesprite5);
	UnloadTexture(notesprite6);
	UnloadTexture(notesprite7);
	UnloadTexture(floortest1);
	UnloadTexture(playersprite1);
	UnloadTexture(playersprite2);
	UnloadTexture(playersprite3);
	UnloadTexture(playersprite4);
	

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
