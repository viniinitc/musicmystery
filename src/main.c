#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir




typedef struct{

	int typenote;
	int posny;
	int posnx;
	int direct;
	int turnr;
	Rectangle rect;
	Vector2 vect;
	Texture2D sprite;
	bool collision;

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

			x[i].posnx += 80;
			x[i].posny += 50;
			
		}
		if (mov == 3){
			x[i].posnx -= 80;
			x[i].posny += 50;
		}
		if (mov == 2){
			x[i].posnx += 80;
			x[i].posny -= 50;
		}
		if (mov == 4){
			x[i].posnx -= 80;
			x[i].posny -= 50;
		}
		
	}

}

typedef struct{

	int hp;
	int type;
	int enemyx;
	int enemyy;

	Texture2D sprite;
	Vector2 vetor;
	Rectangle rect;
	bool collision;

}enemytype;



int main ()
{
	notes notelist[7];
	enemytype enemies[7];

	int exitladderx = 1160;
	int exitladdery = 180;
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
	Vector2 enemiespossiblepossitions[7];

	bool collisionball1 = false;
	bool collisionball2 = false;
	bool collisionball3 = false;
	bool collisionball4 = false;

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 900, "Hello Raylib");
	SetTargetFPS(60);

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	
	Texture playersprite3 = LoadTexture("playersprite3.png");
	Texture playersprite4 = LoadTexture("playersprite4.png");
	Texture exitladder = LoadTexture("ladder.png");
	Texture playersoul = LoadTexture("playersoul.png");
	notelist[0].sprite = LoadTexture("do.png");
	notelist[1].sprite = LoadTexture("re.png");
	notelist[2].sprite = LoadTexture("mi.png");
	notelist[3].sprite = LoadTexture("fa.png");
	notelist[4].sprite = LoadTexture("sol.png");
	notelist[5].sprite = LoadTexture("la.png");
	notelist[6].sprite = LoadTexture("si.png");
	enemies[0].sprite = LoadTexture("enemy1.png");
	enemies[1].sprite = LoadTexture("enemy2.png");
	enemies[2].sprite = LoadTexture("enemy3.png");
	enemies[3].sprite = LoadTexture("enemy4.png");
	enemies[4].sprite = LoadTexture("enemy5.png");
	enemies[5].sprite = LoadTexture("enemy6.png");
	enemies[6].sprite = LoadTexture("enemy7.png");
	Texture2D floortest1 = LoadTexture("floor1.png");
	Texture2D playersprite1 = LoadTexture("playeranimation1.png");
	Texture2D playersprite2 = LoadTexture("playeranimation2.png");
	
	Timer turntimer = {0};
	Timer notetimer = {0};

	
	float turnduration = 1.0f;
	float notetimerduration = turnduration/20;
	float animationdur = 5.0f;

	Vector2 position = { 350.0f, 280.0f};
	Vector2 positionplayer = { 30.0f, 337.0f};
	Rectangle frameRec = {0.0f, 0.0f, (float)floortest1.width/16, (float)floortest1.height};
	Rectangle frameRecplayer = {0.0f, 0.0f, (float)playersprite1.width/16, (float)playersprite1.height};
	Rectangle frameRecplayer2 = {0.0f, 0.0f, (float)playersprite2.width/16, (float)playersprite2.height};
	int soulposx = (int)positionplayer.x; 
	int soulposy = (int)positionplayer.y;

	
	int currentFrame = 0;
	int framesCounter = 0;
	int framesSpeed = 12;

	for (int i = 0; i < 7; i++){
		enemiespossiblepossitions[i].x = 600.0 + (float)(i * 80);
		enemiespossiblepossitions[i].y = 0.0 + (float)(i * 50);
	}


	for (int i = 0; i < 7; i++){
		notelist[i].typenote = i+1;
		notelist[i].rect.height = (float)notelist[i].sprite.height;
		notelist[i].rect.width = (float)notelist[i].sprite.width/16;
		notelist[i].rect.x = 0.0f;
		notelist[i].rect.y = 0.0f;
		notelist[i].turnr = 0;
		notelist[i].collision = false;
		enemies[i].type = i+1;
		enemies[i].hp = 2;
		enemies[i].rect.x = 0.0f;
		enemies[i].rect.y = 0.0f;
		enemies[i].rect.height = (float)enemies[i].sprite.height;
		enemies[i].rect.width = (float)enemies[i].sprite.width;
		enemies[i].collision = false;
		enemies[i].enemyx = enemiespossiblepossitions[i].x;
		enemies[i].enemyy = enemiespossiblepossitions[i].y;
		enemies[i].vetor.x = (float)enemies[i].enemyx ;
		enemies[i].vetor.y = (float)enemies[i].enemyy ;
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

		
		Vector2 contact1 = {(float)position1x, (float)position1y};
		Vector2 contact2 = {(float)position2x, (float)position2y};
		Vector2 contact3 = {(float)position3x, (float)position3y};
		Vector2 contact4 = {(float)position4x, (float)position4y};
		for (int i = 0; i < 7; i++){
			for (int j = 0; j < 7; j++){
				notelist[j].vect.x = (float)notelist[j].posnx;
				notelist[j].vect.y = (float)notelist[j].posny;
				enemies[i].collision = CheckCollisionCircles(notelist[j].vect, 100.0f ,enemies[i].vetor,100.0f);
			}	

			
		}
		
		collisionball1 = CheckCollisionPointCircle(GetMousePosition(),contact1, 20.0);
		collisionball2 = CheckCollisionPointCircle(GetMousePosition(),contact2, 20.0);
		collisionball3 = CheckCollisionPointCircle(GetMousePosition(),contact3, 20.0);
		collisionball4 = CheckCollisionPointCircle(GetMousePosition(),contact4, 20.0);
		
		framesCounter++;

		if (framesCounter >= (60/framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;

            if (currentFrame > 15) currentFrame = 0;

            frameRec.x = (float)currentFrame*(float)floortest1.width/16;
			frameRecplayer.x = (float)currentFrame*(float)playersprite1.width/16;
			frameRecplayer2.x = (float)currentFrame*(float)playersprite1.width/16;
			for (int i = 0; i < 7; i++){
				notelist[i].rect.x = (float)currentFrame*(float)notelist[i].sprite.width/16;
			}
			
        }
		
		//remember to update player movement 
		if (TimerDone(&turntimer)){
			if(IsKeyPressed(KEY_DOWN)) {
				positionplayer.x += 80;
				positionplayer.y += 50;
				dir = 1;
				turn++;
				StartTimer(&turntimer,turnduration);
				turncom = turn;
				StartTimer(&notetimer, notetimerduration);
				shootmove(notelist);
			}
			if(IsKeyPressed(KEY_UP)) {
				positionplayer.x -= 80;
				positionplayer.y -= 50;
				dir = 4;
				turn++;
				StartTimer(&turntimer,turnduration);
				turncom = turn;
				shootmove(notelist);
			}
			if(IsKeyPressed(KEY_RIGHT)) {
				positionplayer.x += 80;
				positionplayer.y -= 50;
				dir = 2;
				turn++;
				StartTimer(&turntimer,turnduration);
				turncom = turn;
				shootmove(notelist);
			}
			if(IsKeyPressed(KEY_LEFT)) {
				positionplayer.x -= 80;
				positionplayer.y += 50;
				dir = 3;
				turn++;
				StartTimer(&turntimer,turnduration);
				turncom = turn;
				shootmove(notelist);
			}
			
			if(IsKeyPressed(KEY_SPACE) && turn != 0){
				turn++;
				
				
				
				notelist[notecheck].direct = dir;
				notelist[notecheck].posnx = positionplayer.x;
				notelist[notecheck].posny = positionplayer.y;
				if(dir == 1) {
					notelist[notecheck].posny += 70;
					notelist[notecheck].posnx += 30;
				}
				if(dir == 2) {
					notelist[notecheck].posny += 70;
					notelist[notecheck].posnx += 35;
				}
				if(dir == 3) {
					notelist[notecheck].posny += 70;
					notelist[notecheck].posnx += 30;
				}
				if(dir == 4) {
					notelist[notecheck].posny += 80;
					notelist[notecheck].posnx += 30;
				}
				notelist[notecheck].turnr = turn;
				

				StartTimer(&turntimer,turnduration);
				shootmove(notelist); 
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
		for (int i = 0; i < 7; i++){
			notelist[i].vect.x = notelist[i].posnx;
			notelist[i].vect.y = notelist[i].posny;
		}
		
		DrawTexture(floortest1, 20, 40, WHITE);
		DrawTexture(exitladder, exitladderx, exitladdery, WHITE);
		DrawTexture(playersoul, soulposx, soulposy, WHITE);
		if (dir == 1) DrawTextureRec(playersprite1, frameRecplayer, positionplayer, WHITE);
		if (dir == 3) DrawTextureRec(playersprite2, frameRecplayer2, positionplayer, WHITE);
		if (dir == 2) DrawTexture(playersprite3, positionplayer.x, positionplayer.y, WHITE);
		if (dir == 4) DrawTexture(playersprite4, positionplayer.x, positionplayer.y, WHITE);
		for (int i = 0; i < 7; i++){
			if (notelist[i].turnr > 0) DrawTextureRec(notelist[i].sprite, notelist[i].rect,notelist[i].vect, WHITE);
		}
		//DrawTexture(notesprite1, notelist[0].posnx, notelist[0].posny, WHITE); //draw texture also to figure out the individual note movement

	
		//temporary mouse text so i can figure out positions
		DrawTextEx(GetFontDefault(), TextFormat("[%i, %i]", GetMouseX(), GetMouseY()),
                Vector2Add(GetMousePosition(), (Vector2){ -44, -24 }), 20, 2, BLACK);


		for (int i = 0; i < 7; i++){
			//if (!enemies[i].collision) DrawTexture(enemies[i].sprite,enemies[i].enemyx,enemies[i].enemyy,WHITE); commenting this for now so i can figure collision out
			DrawTexture(enemies[i].sprite,enemies[i].enemyx,enemies[i].enemyy,WHITE);
			if (enemies[i].collision) DrawText("contact", 100, 100, 100, BLACK);
		}
		if (collisionball1) DrawText("1", 100, 100, 100, BLACK);
		if (collisionball2) DrawText("2", 100, 100, 100, BLACK);
		if (collisionball3) DrawText("3", 100, 100, 100, BLACK);
		if (collisionball4) DrawText("4", 100, 100, 100, BLACK);

		DrawCircle(position1x ,position1y, 20, GRAY);

		DrawCircle(position2x ,position2y , 20, GRAY);

		DrawCircle(position3x,position3y, 20, GRAY);

		DrawCircle(position4x ,position4y , 20, GRAY);
		

		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	for (int i = 0; i < 7; i++){
		UnloadTexture(notelist[i].sprite);
	}

	UnloadTexture(floortest1);
	UnloadTexture(playersprite1);
	UnloadTexture(playersprite2);
	UnloadTexture(playersprite3);
	UnloadTexture(playersprite4);
	

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
