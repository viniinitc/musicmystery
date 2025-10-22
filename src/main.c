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
	struct notes* next;

}notes;

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

void shootmove(notes* x){

	for(int i = 0; i < 7; i++){
		
		int mov = x->direct;
			
		if (mov == 1){
			x->posny += 64;
		}
		if (mov == 3){
			x->posny -= 64;
		}
		if (mov == 2){
			x->posnx += 64;
		}
		if (mov == 4){
			x->posnx -= 64;
		}
		
		x = x->next;
	}

}

typedef struct{

	int hp;
	int type;
	struct enemytype* next;

}enemytype;



int main ()
{
	int notelist[7];
	int posx=64;
	int posy=640;
	int dir = 1;
	int playerhp = 3;
	int turn = 0;
	int turncom;
	int notecheck = 0;

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
	Texture notesprite1 = LoadTexture("do.png");
	Texture notesprite2 = LoadTexture("re.png");
	Texture notesprite3 = LoadTexture("mi.png");
	Texture notesprite4 = LoadTexture("fa.png");
	Texture notesprite5 = LoadTexture("so.png");
	Texture notesprite6 = LoadTexture("la.png");
	Texture notesprite7 = LoadTexture("si.png");

	Timer turntimer = {0};

	float turnduration = 1.0f;
	float animationdur = 5.0f;
	
	notes* head = (notes*)malloc(sizeof(notes));
	head->typenote = 1;
	head->next = NULL;
	notes* n = head;
	notes* new = NULL;
	
	for (int i = 2; i < 8; i++){
		new = (notes*)malloc(sizeof(notes));
		new->typenote = i;
		new->next = NULL;
		n->next = new;
		n = n->next;
	}
	notes* note1 = head;
	for (int i = 0; i < 7; i++){
		notelist[i] = 0;
	}
	n = head;
	new = head;

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{

		
		
		if (TimerDone(&turntimer)){
			if(IsKeyPressed(KEY_DOWN)) {
				posy += 64;
				dir = 1;
				turn++;
				StartTimer(&turntimer,turnduration);
				turncom = turn;
				shootmove(new);
				new = head;
			}
			if(IsKeyPressed(KEY_UP)) {
				posy -= 64;
				dir = 3;
				turn++;
				StartTimer(&turntimer,turnduration);
				turncom = turn;
				shootmove(new);
				new = head;
			}
			if(IsKeyPressed(KEY_RIGHT)) {
				posx += 64;
				dir = 2;
				turn++;
				StartTimer(&turntimer,turnduration);
				turncom = turn;
				shootmove(new);
				new = head;
			}
			if(IsKeyPressed(KEY_LEFT)) {
				posx -= 64;
				dir = 4;
				turn++;
				StartTimer(&turntimer,turnduration);
				turncom = turn;
				shootmove(new);
				new = head;
			}
			if(IsKeyPressed(KEY_SPACE) && turn != 0){
				turn++;
				
				notecheck++;
				
				n->direct = dir;
				n->posnx = posx;
				n->posny = posy;
				if(dir == 1) n->posny += 64;
				if(dir == 3) n->posny -= 64;
				if(dir == 2) n->posnx += 64;
				if(dir == 4) n->posnx -= 64;
				n->turnr = turn;
				
				//shoot(n);
				//n = n->next;
				if (n == NULL) n = head;
				StartTimer(&turntimer,turnduration);
				shootmove(new);
				new = head; //n = n->next; removed this for now so i can figure out the movement of the individual notes
				turncom = turn;

			}
		}

		if (turncom == turn){

			
		}
		
		
		if (n == NULL) n = head;

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
		DrawCircle(pcolx,pcoly,1, GREEN);
		DrawTexture(notesprite1, n->posnx, n->posny, WHITE); //draw texture also to figure out the individual note movement
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
