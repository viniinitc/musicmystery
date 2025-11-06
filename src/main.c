#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "buttons.h"


#define SQUARENOTES 1

typedef enum GameScreen {

	LOGO,
	STARTSCREEN,
	PAUSE,
	CREDITS,
	HIGHSCORE,
	DEAD,
	GAMEPLAY

}GameScreen;

typedef enum buttontype {

	STORY = 0,
	ENDLESS,
	CONTINUE,
	EXIT,
	RESTART,
	MENU

}buttontype;






typedef struct{

	int typenote;
	float posny;
	float posnx;
	int direct;
	int turnr;
	Rectangle rect;
	Vector2 vect;
	Texture2D sprite;
	bool collision;
	Sound notesound;

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

			x[i].posnx += 1.32;
			x[i].posny += 0.825*SQUARENOTES;
			
		}
		if (mov == 3){
			x[i].posnx -= 1.32;
			x[i].posny += 0.825*SQUARENOTES;
		}
		if (mov == 2){
			x[i].posnx += 1.32;
			x[i].posny -= 0.825*SQUARENOTES;
		}
		if (mov == 4){
			x[i].posnx -= 1.32;
			x[i].posny -= 0.825*SQUARENOTES;
		}
		
	}

}

typedef struct{

	int hp;
	int type;
	int enemyx;
	int enemyy;

	int explosioncheck;
	Vector2 explosionvect;
	int currentExplosionFrame;
	int framesExplosionCounter;
	Rectangle explosionrect;

	
	Texture2D sprite;
	Vector2 vetor;
	Rectangle rect;
	bool collision;
	bool dead;
	

}enemytype;



int main ()
{

	GameScreen currentScreen = LOGO;

	buttontype currentbutton;


	int framecounter = 0;

	notes notelist[7];
	enemytype enemies[7];

	int restart = 0;
	int notebeingshot = 0;
	int notemovement = 0;
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
	int explosioncheck = 0;
	int playerpoint = 0;
	Vector2 enemiespossiblepossitions[15];
	Texture notenoan[7];
	bool collisionball1 = false;
	bool collisionball2 = false;
	bool collisionball3 = false;
	bool collisionball4 = false;

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	int screenwidth = 1280;
	int screenheight = 900;
	InitWindow(screenwidth, screenheight, "MusicMystery");
	InitAudioDevice();
	SetTargetFPS(60);

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");


	
	notelist[0].notesound = LoadSound("Dochange.mp3");
	notelist[1].notesound = LoadSound("Rechange.mp3");
	notelist[2].notesound = LoadSound("Michange.mp3");
	notelist[3].notesound = LoadSound("Fachange.mp3");
	notelist[4].notesound = LoadSound("Solchange.mp3");
	notelist[5].notesound = LoadSound("Lachange.mp3");
	notelist[6].notesound = LoadSound("Sichange.mp3");

	Sound enemydeath = LoadSound("enemydeath.mp3");

	Music startmusic = LoadMusicStream("gamesong.mp3");
	// Load a texture from the resources directory
	
	Texture playersprite3 = LoadTexture("playersprite3.png");
	Texture playersprite4 = LoadTexture("playersprite4.png");
	Texture exitladder = LoadTexture("ladder.png");
	Texture playersoul = LoadTexture("playersoul.png");


	notenoan[0] = LoadTexture("donoan.png");
	notenoan[1] = LoadTexture("renoan.png");
	notenoan[2] = LoadTexture("minoan.png");
	notenoan[3] = LoadTexture("fanoan.png");
	notenoan[4] = LoadTexture("solnoan.png");
	notenoan[5] = LoadTexture("lanoan.png");
	notenoan[6] = LoadTexture("sinoan.png");


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


	Texture2D explosion = LoadTexture("explosion.png");
	Texture2D floortest1 = LoadTexture("floor1.png");
	Texture2D playersprite1 = LoadTexture("playeranimation1.png");
	Texture2D playersprite2 = LoadTexture("playeranimation2.png");
	Texture mylogo = LoadTexture("logo.png");


	Texture2D storybutton = LoadTexture("buttonstory.png");
	Texture2D endlessbutton = LoadTexture("buttonendless.png");
	Texture2D continuebutton = LoadTexture("buttoncontinue.png");
	Texture2D exitbutton = LoadTexture("buttonexit.png");
	Texture2D restartbutton = LoadTexture("buttonrestart.png");
	Texture2D menubutton = LoadTexture("buttonmenu.png");
	Texture2D creditsbutton = LoadTexture("buttoncredits.png");

	Timer turntimer = {0};
	Timer notetimer = {0};

	
	float turnduration = 1.0f;
	float notetimerduration = turnduration/20;
	float animationdur = 5.0f;

	Vector2 position = { 350.0f, 280.0f};
	Vector2 positionplayer = { 30.0f, 337.0f};
	
	Rectangle frameRec = {0.0f, 0.0f, (float)floortest1.width/16, (float)floortest1.height};
	Rectangle explosionrec = {0.0f, 0.0f, (float)explosion.width/16, (float)explosion.height};
	Rectangle frameRecplayer = {0.0f, 0.0f, (float)playersprite1.width/16, (float)playersprite1.height};
	Rectangle frameRecplayer2 = {0.0f, 0.0f, (float)playersprite2.width/16, (float)playersprite2.height};


	//buttons--------------------------------------------------------------------------------

	Rectangle buttonstorybound = {screenwidth/2.0f - storybutton.width/3/2.0f, screenheight/2.0f - storybutton.height/2.0f, (float)storybutton.width/3, (float)storybutton.height};
	Rectangle buttonstorysource = {0, 0, (float)storybutton.width/3, (float)storybutton.height};
	int buttonstorystate = 0;
	bool buttonstoryactive = false;

	Rectangle buttonendlessbound = {screenwidth/2.0f - endlessbutton.width/3/2.0f, screenheight/2.0f - endlessbutton.height/2.0f + (endlessbutton.height+20), (float)endlessbutton.width/3, (float)endlessbutton.height};
	Rectangle buttonendlesssource = {0, 0, (float)endlessbutton.width/3, (float)endlessbutton.height};
	int buttonendlessstate = 0;
	bool buttonendlessactive = false;

	Rectangle buttoncontinuebound = {screenwidth/2.0f - continuebutton.width/3/2.0f - ((continuebutton.width/3)+20), screenheight/2.0f - continuebutton.height/2.0f, (float)continuebutton.width/3, (float)continuebutton.height};
	Rectangle buttoncontinuesource = {0, 0, (float)continuebutton.width/3, (float)continuebutton.height};
	int buttoncontinuestate = 0;
	bool buttoncontinueactive = false;

	Rectangle buttonexitbound = {screenwidth/2.0f - exitbutton.width/3/2.0f , screenheight/2.0f - exitbutton.height/2.0f + ((exitbutton.height+20)*3), (float)exitbutton.width/3, (float)exitbutton.height};
	Rectangle buttonexitsource = {0, 0, (float)exitbutton.width/3, (float)exitbutton.height};
	int buttonexitstate = 0;
	bool buttonexitactive = false;

	Rectangle buttonrestartbound = {screenwidth/2.0f - restartbutton.width/3/2.0f , screenheight/2.0f - restartbutton.height/2.0f, (float)restartbutton.width/3, (float)restartbutton.height};
	Rectangle buttonrestartsource = {0, 0, (float)restartbutton.width/3, (float)restartbutton.height};
	int buttonrestartstate = 0;
	bool buttonrestartactive = false;

	Rectangle buttonmenubound = {screenwidth/2.0f - menubutton.width/3/2.0f + ((menubutton.width/3)+20), screenheight/2.0f - menubutton.height/2.0f, (float)menubutton.width/3, (float)menubutton.height};
	Rectangle buttonmenusource = {0, 0, (float)menubutton.width/3, (float)menubutton.height};
	int buttonmenustate = 0;
	bool buttonmenuactive = false;

	Rectangle buttoncreditsbound = {screenwidth/2.0f - creditsbutton.width/3/2.0f, screenheight/2.0f - creditsbutton.height/2.0f + ((creditsbutton.height+20)*2), (float)creditsbutton.width/3, (float)creditsbutton.height};
	Rectangle buttoncreditssource = {0, 0, (float)creditsbutton.width/3, (float)creditsbutton.height};
	int buttoncreditsstate = 0;
	bool buttoncreditsactive = false;

	//buttons---------------------------------



	int soulposx = (int)positionplayer.x; 
	int soulposy = (int)positionplayer.y;

	
	int currentFrame = 0;
	int framesCounter = 0;
	int framesSpeed = 12;

	int currentExplosionFrame = 0;
	int framesExplosionCounter = 0;

	

	int contpos = 0;

	for (int j = 0; j < 2; j++){
		for (int i = 0; i < 7; i++){
			
			enemiespossiblepossitions[contpos].x = 600.0 - (float)(j* 80) + (float)(i * 80);
			enemiespossiblepossitions[contpos].y = 0.0 + (float)(j * 50) + (float)(i * 50);
			contpos++;
		}
	}
	enemiespossiblepossitions[contpos].x = 600.0 - 80.0 + (float)(7 * 80);
	enemiespossiblepossitions[contpos].y = 0.0 + 50.0 + (float)(7 * 50);


	int *seq = LoadRandomSequence((unsigned int) 7, 0, 14);
	



	for (int i = 0; i < 7; i++){
		notelist[i].typenote = i+1;
		notelist[i].rect.height = (float)notelist[i].sprite.height;
		notelist[i].rect.width = (float)notelist[i].sprite.width/16;
		notelist[i].rect.x = 0.0f;
		notelist[i].rect.y = 0.0f;
		notelist[i].turnr = 0;
		notelist[i].collision = false;
		enemies[i].type = i+1;
		enemies[i].hp = 1;
		enemies[i].rect.x = 0.0f;
		enemies[i].rect.y = 0.0f;
		enemies[i].rect.height = (float)enemies[i].sprite.height;
		enemies[i].rect.width = (float)enemies[i].sprite.width;
		enemies[i].collision = false;
		enemies[i].dead = false;
		enemies[i].enemyx = enemiespossiblepossitions[seq[i]].x;
		enemies[i].enemyy = enemiespossiblepossitions[seq[i]].y;
		enemies[i].vetor.x = (float)enemies[i].enemyx + enemies[i].sprite.width/2;
		enemies[i].vetor.y = (float)enemies[i].enemyy + enemies[i].sprite.height/2;
		enemies[i].explosioncheck = 0;
		enemies[i].framesExplosionCounter = 0;
		enemies[i].currentExplosionFrame = 0;
		enemies[i].explosionrect.x = 0.0f;
		enemies[i].explosionrect.y = 0.0f;
		enemies[i].explosionrect.height = (float)explosion.height;
		enemies[i].explosionrect.width = (float)explosion.width/16;
	}


	int position1x;
	int position1y;
	int position2x;
	int position2y;
	int position3x;
	int position3y;
	int position4x;
	int position4y;

	
	SetExitKey(KEY_NULL);
	bool exitWindowRequested = false;
	bool exitWindow = false;
	Vector2 mousebutn = {0.0f, 0.0f};

	PlayMusicStream(startmusic);

	// game loop
	while (!exitWindow)		
	{

		switch(currentScreen){
			case LOGO:{

				UpdateMusicStream(startmusic);
				framesCounter++;

				if (framesCounter > 120){
					currentScreen = STARTSCREEN;
					framecounter = 0;
				}

			}break;

			case STARTSCREEN:{

				UpdateMusicStream(startmusic);
				mousebutn = GetMousePosition();
				
				buttonstoryactive = false;
				buttonendlessactive = false;
				buttoncreditsactive = false;
				buttonexitactive = false;

				if (CheckCollisionPointRec(mousebutn, buttonstorybound)) {
					if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) buttonstorystate = 2;
					else buttonstorystate = 1;

					if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) buttonstoryactive = true;
				} else buttonstorystate = 0;

				buttonstorysource.x = (float)(buttonstorystate*(storybutton.width/3));

				if (buttonstoryactive) currentScreen = GAMEPLAY;



				if (CheckCollisionPointRec(mousebutn, buttonendlessbound)) {
					if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) buttonendlessstate = 2;
					else buttonendlessstate = 1;

					if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) buttonendlessactive = true;
				} else buttonendlessstate = 0;

				buttonendlesssource.x = (float)(buttonendlessstate*(endlessbutton.width/3));

				if (buttonendlessactive) currentScreen = GAMEPLAY;



				if (CheckCollisionPointRec(mousebutn, buttoncreditsbound)) {
					if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) buttoncreditsstate = 2;
					else buttoncreditsstate = 1;

					if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) buttoncreditsactive = true;
				} else buttoncreditsstate = 0;

				buttoncreditssource.x = (float)(buttoncreditsstate*(creditsbutton.width/3));

				if (buttoncreditsactive) currentScreen = CREDITS;




				if (CheckCollisionPointRec(mousebutn, buttonexitbound)) {
					if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) buttonexitstate = 2;
					else buttonexitstate = 1;

					if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) buttonexitactive = true;
				} else buttonexitstate = 0;

				buttonexitsource.x = (float)(buttonexitstate*(exitbutton.width/3));

				if (buttonexitactive)exitWindow = true;
				


			}break;
			
			case GAMEPLAY:{

				if(WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) {
					exitWindowRequested = true;
					currentScreen = PAUSE;
				}


	
				



				Vector2 contact1 = {(float)position1x, (float)position1y};
				Vector2 contact2 = {(float)position2x, (float)position2y};
				Vector2 contact3 = {(float)position3x, (float)position3y};
				Vector2 contact4 = {(float)position4x, (float)position4y};
				for (int i = 0; i < 7; i++){

					notelist[i].vect.x = (float)notelist[i].posnx + 32;
					notelist[i].vect.y = (float)notelist[i].posny + 32;
					enemies[i].collision = CheckCollisionCircles(notelist[i].vect, 20.0f,enemies[i].vetor, 20.0f);
					notelist[i].collision = CheckCollisionCircles(notelist[i].vect, 20.0f, enemies[i].vetor,20.0f);
					if (notelist[i].collision) notelist[i].turnr = 0;
					if (enemies[i].collision) enemies[i].dead = true;
					if (enemies[i].dead) {
						enemies[i].explosioncheck++;
						if (enemies[i].explosioncheck == 1){
							enemies[i].explosionvect = enemies[i].vetor;
							enemies[i].explosionvect.x -= 84;
							enemies[i].explosionvect.y -= 94;
							PlaySound(enemydeath);
						}
						enemies[i].vetor.x = -100;
						enemies[i].vetor.y = -100;
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

				
				

				for (int i = 0; i < 7; i++){
					if (enemies[i].framesExplosionCounter >= (60/framesSpeed))
					{
					
						enemies[i].framesExplosionCounter = 0;

						enemies[i].currentExplosionFrame++;

						if (enemies[i].currentExplosionFrame > 15) {
							enemies[i].currentExplosionFrame = 15;
							playerpoint ++;
						}

						enemies[i].explosionrect.x = (float)enemies[i].currentExplosionFrame*(float)explosion.width/16;
					}
					if(enemies[i].dead)enemies[i].framesExplosionCounter++;
				}

				


				
				//remember to update player movement 
				if (TimerDone(&turntimer)){
					if(IsKeyPressed(KEY_DOWN)) {

						
						if (dir == 1) {
							
							
							notebeingshot = 1;
							turn++;
							StartTimer(&turntimer,turnduration);
							turncom = turn;
							StartTimer(&notetimer, notetimerduration);
							shootmove(notelist);
						} else {
							dir = 1;
						}
					}
					if(IsKeyPressed(KEY_UP)) {

						if (dir == 4){
							notebeingshot = 1;
							turn++;
							StartTimer(&turntimer,turnduration);
							turncom = turn;
							shootmove(notelist);
						} else {
							dir = 4;
						}
					}
					if(IsKeyPressed(KEY_RIGHT)) {
						

						if (dir == 2){
							notebeingshot = 1;
							turn++;
							StartTimer(&turntimer,turnduration);
							turncom = turn;
							shootmove(notelist);
						} else {
							dir = 2;
						}
					}
					if(IsKeyPressed(KEY_LEFT)) {

						if (dir == 3){
							notebeingshot = 1;
							turn++;
							StartTimer(&turntimer,turnduration);
							turncom = turn;
							shootmove(notelist);
						} else {
							dir = 3;
						}
					}
					
					if(IsKeyPressed(KEY_SPACE) && turn != 0){
						
						notebeingshot = 0;
						notemovement = 1;
						
						if(notelist[notecheck].turnr == 0 || (notelist[notecheck].posnx > screenwidth) || (notelist[notecheck].posny > screenheight) || (notelist[notecheck].posnx < 0) || (notelist[notecheck].posny < 0)){ //turn - notelist[notecheck].turnr >= 3
							turn++;
							notelist[notecheck].direct = dir;
							notelist[notecheck].posnx = positionplayer.x;
							notelist[notecheck].posny = positionplayer.y;
							if(dir == 1) {
								notelist[notecheck].posny += 120;
								notelist[notecheck].posnx += 110;
							}
							if(dir == 2) {
								notelist[notecheck].posny += 20;
								notelist[notecheck].posnx += 115;
							}
							if(dir == 3) {
								notelist[notecheck].posny += 120;
								notelist[notecheck].posnx -= 50;
							}
							if(dir == 4) {
								notelist[notecheck].posny += 30;
								notelist[notecheck].posnx -= 50;
							}
							notelist[notecheck].turnr = turn;
							
							PlaySound(notelist[notecheck].notesound);

							StartTimer(&turntimer,turnduration);
							shootmove(notelist); 
							turncom = turn;
						}

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
				
				//smooth movement of the player
				if (!TimerDone(&turntimer) && notebeingshot){
					int newposition = positionplayer.x+80;
					if (dir == 1 ){
						positionplayer.x += 1.32;
						positionplayer.y += 0.825;
					}
					if (dir == 2){
						positionplayer.x += 1.32;
						positionplayer.y -= 0.825;

					}
					if (dir == 3){
						positionplayer.x -= 1.32;
						positionplayer.y += 0.825;
					}
					if (dir == 4){

						positionplayer.x -= 1.32;
						positionplayer.y -= 0.825;

					}
				}

				//smooth movement of the notes continue later
				if (!TimerDone(&turntimer) && notemovement){
				
					shootmove(notelist);


				}



				UpdateTimer(&turntimer);

				int pcolx = posx+32; 
				int pcoly = posy+64;

			}break;

			case PAUSE:{

				mousebutn = GetMousePosition();

				buttonmenuactive = false;
				buttoncontinueactive = false;
				buttonexitactive = false;

				if (CheckCollisionPointRec(mousebutn, buttoncontinuebound)) {
					if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) buttoncontinuestate = 2;
					else buttoncontinuestate = 1;

					if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) buttoncontinueactive = true;
				} else buttoncontinuestate = 0;

				buttoncontinuesource.x = (float)(buttoncontinuestate*(continuebutton.width/3));

				if (buttoncontinueactive) currentScreen = GAMEPLAY;



				if (CheckCollisionPointRec(mousebutn, buttonexitbound)) {
					if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) buttonexitstate = 2;
					else buttonexitstate = 1;

					if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) buttonexitactive = true;
				} else buttonexitstate = 0;

				buttonexitsource.x = (float)(buttonexitstate*(exitbutton.width/3));

				if (buttonexitactive)exitWindow = true;

				if (CheckCollisionPointRec(mousebutn, buttonmenubound)) {
					if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) buttonmenustate = 2;
					else buttonmenustate = 1;

					if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) buttonmenuactive = true;
				} else buttonmenustate = 0;

				buttonmenusource.x = (float)(buttonmenustate*(menubutton.width/3));

				if (buttonmenuactive) {
					currentScreen = STARTSCREEN;
					restart = 1;
				}



			}break;

			case DEAD:{

			}break;

			case CREDITS:{

			}break;

			case HIGHSCORE:{

			}break;
			default: break;
		}
		
		
		
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(WHITE);

		// draw some text using the default font
		DrawText("Hello Raylib ", 200,200,20,WHITE);

		// draw our texture to the screen
		switch(currentScreen){
			case LOGO:{
				DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
				DrawTexture(mylogo, (screenwidth/2)-(mylogo.width/2), (screenheight/2)-(mylogo.height/2), WHITE);
                DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);
			}break;

			case STARTSCREEN:{
				DrawRectangle(0, 0,	screenwidth, screenheight, GREEN);
                DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
                DrawText("PRESS SPACEo to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);

				DrawTextureRec(storybutton, buttonstorysource, (Vector2){ buttonstorybound.x, buttonstorybound.y }, WHITE);

				DrawTextureRec(endlessbutton, buttonendlesssource, (Vector2){ buttonendlessbound.x, buttonendlessbound.y}, WHITE);

				DrawTextureRec(creditsbutton, buttoncreditssource, (Vector2){ buttoncreditsbound.x, buttoncreditsbound.y}, WHITE);

				DrawTextureRec(exitbutton, buttonexitsource, (Vector2){ buttonexitbound.x, buttonexitbound.y}, WHITE);

			}break;

			case GAMEPLAY:{
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
					if (notelist[i].turnr > 0  && !notelist[i].collision) DrawTextureRec(notelist[i].sprite, notelist[i].rect,notelist[i].vect, WHITE);
					//if (notelist[i].turnr > 0) DrawCircle(notelist[i].vect.x + 32,notelist[i].vect.y + 32, 20.0f, BLUE);
				}
				//DrawTexture(notesprite1, notelist[0].posnx, notelist[0].posny, WHITE); //draw texture also to figure out the individual note movement

			
				//temporary mouse text so i can figure out positions
				DrawTextEx(GetFontDefault(), TextFormat("[%i, %i]", GetMouseX(), GetMouseY()),
						Vector2Add(GetMousePosition(), (Vector2){ -44, -24 }), 20, 2, BLACK);


				for (int i = 0; i < 7; i++){
					
					if(!enemies[i].dead)DrawTexture(enemies[i].sprite,enemies[i].enemyx,enemies[i].enemyy,WHITE);
					if (enemies[i].dead)DrawTextureRec(explosion, enemies[i].explosionrect, enemies[i].explosionvect , WHITE);
					
					if (notelist[i].collision) DrawText("contact", 100, 100, 100, BLACK);
				}
				if (collisionball1) DrawText("1", 100, 100, 100, BLACK);
				if (collisionball2) DrawText("2", 100, 100, 100, BLACK);
				if (collisionball3) DrawText("3", 100, 100, 100, BLACK);
				if (collisionball4) DrawText("4", 100, 100, 100, BLACK);

				DrawCircle(position1x ,position1y, 20, GRAY);

				DrawCircle(position2x ,position2y , 20, GRAY);

				DrawCircle(position3x,position3y, 20, GRAY);

				DrawCircle(position4x ,position4y , 20, GRAY);
				
				for (int i = 0; i < 7; i++){
					if (i == notecheck) DrawRectangle(30 + 71*i, 800, 70, 70, RED);
					DrawTexture(notenoan[i], 30 + 71*i, 803, WHITE);
					DrawRectangleLines(30 + 71*i, 800, 70, 70, MAROON);
				}


				
				
				
			}break;

			case PAUSE:{
				DrawRectangle(0, 0, screenwidth, screenheight, RED);
				DrawText("are you sure you want to exit program?", 40, 180, 30, WHITE);

				DrawTextureRec(continuebutton, buttoncontinuesource, (Vector2){ buttoncontinuebound.x, buttoncontinuebound.y}, WHITE);

				DrawTextureRec(menubutton, buttonmenusource, (Vector2){ buttonmenubound.x, buttonmenubound.y}, WHITE);

				DrawTextureRec(exitbutton, buttonexitsource, (Vector2){ buttonexitbound.x, buttonexitbound.y}, WHITE);

			}break;

			case CREDITS:{

			}break;

			case HIGHSCORE:{

			}break;

			case DEAD:{

			}break;
			default: break;
		}

		
		
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	for (int i = 0; i < 7; i++){
		UnloadTexture(notelist[i].sprite);
		UnloadTexture(enemies[i].sprite);
		UnloadTexture(notenoan[i]);
		UnloadSound(notelist[i].notesound);
	}


	UnloadTexture(mylogo);
	UnloadTexture(playersoul);
	UnloadTexture(exitladder);
	UnloadTexture(floortest1);
	UnloadTexture(playersprite1);
	UnloadTexture(playersprite2);
	UnloadTexture(playersprite3);
	UnloadTexture(playersprite4);
	


	// destroy the window and cleanup the OpenGL context
	CloseAudioDevice();
	CloseWindow();
	return 0;
}
