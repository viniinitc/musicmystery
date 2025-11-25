#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir


#define SQUARENOTES 1

typedef enum GameScreen {

	LOGO,
	STARTSCREEN,
	PAUSE,
	CREDITS,
	HIGHSCORE,
	DEAD,
	GAMEPLAY,
	TRANSITION,

}GameScreen;

typedef enum buttontype {

	STORY = 0,
	ENDLESS,
	CONTINUE,
	EXIT,
	RESTART,
	MENU

}buttontype;


typedef struct squarefloor{

	Vector2 position;
	bool isenemyhere;
	bool isplayerhere;
	bool isobstaclehere;
	bool isnotehere;
	

}squarefloor;




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
	float enemyx;
	float enemyy;
	int direction;
	int movement[20];

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
	bool canreach;
	bool exists; 
	int i;
	int j;
	
	

}enemytype;




void enemymovement(enemytype* enemy){

	for (int i = 0; i < 7; i++){
		int move = 0;
		if (enemy[i].direction == 1){
			if (move == 1) (enemy[i].i++);
			enemy[i].enemyx += 1.32;
			enemy[i].enemyy += 0.825f;
			enemy[i].vetor.x += 1.32;
			enemy[i].vetor.y += 0.825;
			move++;
		} else if (enemy[i].direction == 3){
			if (move == 1) (enemy[i].j--);
			enemy[i].enemyx -= 1.32;
			enemy[i].enemyy += 0.825f;
			enemy[i].vetor.x -= 1.32;
			enemy[i].vetor.y += 0.825;
			move++;
		} else if (enemy[i].direction == 2){
			if (move == 1) (enemy[i].j++);
			enemy[i].enemyx += 1.32;
			enemy[i].enemyy -= 0.825f;
			enemy[i].vetor.x += 1.32;
			enemy[i].vetor.y -= 0.825;
			move++;
		} else if (enemy[i].direction == 4){
			if (move == 1) (enemy[i].i--);
			enemy[i].enemyx -= 1.32;
			enemy[i].enemyy -= 0.825f;
			enemy[i].vetor.x -= 1.32;
			enemy[i].vetor.y -= 0.825;
			move++;
		}
	}

}




int main ()
{

	GameScreen currentScreen = LOGO;

	buttontype currentbutton;


	int framecounter = 0;

	notes notelist[7];
	enemytype enemies[7];
	squarefloor** floor = (squarefloor**)malloc(8 * sizeof(squarefloor*));
	for (int i = 0; i < 8; i++) floor[i] = (squarefloor*)malloc(8 * sizeof(squarefloor));

	int restart = 0;

	int playermovemet = 0;
	int notebeingshot = 0;
	int notemovement = 0;
	int enemymove = 1;
	int enemymovementcounter = 0;



	int exitladderx = 1160;
	int exitladdery = 180;
	int posx=64;
	int posy=640;
	int firstblockx = 48;
	int firstblocky = 708;

	int dir = 1;

	int playerhp = 3;
	int playeri = 0;
	int playerj = 0;
	int turn = 0;
	int turncom;

	int notecheck = 0;
	int cont = 0;
	int explosioncheck = 0;
	int playerpoint = 0;
	Vector2 enemiespossiblepossitions[7];
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
	Sound deadmusic = LoadSound("DEFEAT.mp3");

	Music startmusic = LoadMusicStream("gamesong.mp3");
	
	// Load a texture from the resources directory
	
	


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


	Texture obstacle = LoadTexture("obstacles.png");
	Texture exitladder = LoadTexture("ladder.png");
	Texture2D explosion = LoadTexture("explosion.png");
	Texture2D floortest1 = LoadTexture("floor1.png");


	Texture playersoul = LoadTexture("playersoul.png");
	Texture playersprite3 = LoadTexture("playersprite3.png");
	Texture playersprite4 = LoadTexture("playersprite4.png");
	
	Texture2D playersprite1 = LoadTexture("playeranimation1.png");
	Texture2D playersprite2 = LoadTexture("playeranimation2.png");
	Texture2D playerjumpdown = LoadTexture("playeranimationjumpdown.png");
	Texture2D playerjumpright = LoadTexture("playersanimationjumpright.png");
	Texture2D playerjumpup = LoadTexture("playersanimationjumpup.png");
	Texture2D playerjumpleft = LoadTexture("playeranimationjumpleft.png");


	Texture mylogo = LoadTexture("logo.png");


	Texture2D storybutton = LoadTexture("buttonstory.png");
	Texture2D endlessbutton = LoadTexture("buttonendless.png");
	Texture2D continuebutton = LoadTexture("buttoncontinue.png");
	Texture2D exitbutton = LoadTexture("buttonexit.png");
	Texture2D restartbutton = LoadTexture("buttonrestart.png");
	Texture2D menubutton = LoadTexture("buttonmenu.png");
	Texture2D creditsbutton = LoadTexture("buttoncredits.png");

	Texture2D background1 = LoadTexture("sky.png");
	Texture2D background2 = LoadTexture("sky2.png");
	Texture2D background3 = LoadTexture("sky3.png");

	float scrollingback = 0.0f;
	float scrollingmid = 0.0f;
	float scrollingfore = 0.0f;


	Timer turntimer = {0};
	Timer notetimer = {0};

	
	float turnduration = 1.0f;
	float notetimerduration = turnduration/20;
	float animationdur = 5.0f;

	Vector2 position = { 350.0f, 280.0f};
	Vector2 positionplayer = { 30.0f, 327.0f};

	Rectangle playerjumprectdown = {0.0f, 0.0f, (float)playerjumpdown.width/16, (float)playerjumpdown.height};
	Rectangle playerjumprectup = {0.0f, 0.0f, (float)playerjumpup.width/16, (float)playerjumpup.height};
	Rectangle playerjumprectright = {0.0f, 0.0f, (float)playerjumpright.width/16, (float)playerjumpright.height};
	Rectangle playerjumprectleft = {0.0f, 0.0f, (float)playerjumpleft.width/16, (float)playerjumpleft.height};
	
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

	Rectangle buttonexitboundnew = {screenwidth/2.0f - exitbutton.width/3/2.0f - ((exitbutton.width/3)-20) , screenheight/2.0f - exitbutton.height/2.0f + ((exitbutton.height+20)*3), (float)exitbutton.width/3, (float)exitbutton.height};
	Rectangle buttonexitsourcenew = {0, 0, (float)exitbutton.width/3, (float)exitbutton.height};

	Rectangle buttonmenuboundnew = {screenwidth/2.0f - menubutton.width/3/2.0f + ((menubutton.width/3)+20), screenheight/2.0f - exitbutton.height/2.0f + ((exitbutton.height+20)*3), (float)menubutton.width/3, (float)exitbutton.height};
	Rectangle buttonmenusourcenew = {0, 0, (float)menubutton.width/3, (float)menubutton.height};

	//buttons---------------------------------



	int soulposx = (int)positionplayer.x; 
	int soulposy = (int)positionplayer.y;

	
	int currentFrame = 0;
	int framesCounter = 0;
	int framesSpeed = 15;


	int currentplayerjumpframe = 0;
	int playerjumpcounter = 0;

	int currentExplosionFrame = 0;
	int framesExplosionCounter = 0;


	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			floor[i][j].isenemyhere = true;
			floor[i][j].isnotehere = false;
			floor[i][j].isobstaclehere = false;
			floor[i][j].isplayerhere = false;
			floor[i][j].position.x = 100.0f + ((j+i)*77);
			floor[i][j].position.y = 445.0f - ((j)*50) + (i*50);	
		}
	}

	int map = 1; //GetRandomValue(1, 2);

	if (map == 1){
		floor[0][2].isobstaclehere = true;
		floor[0][5].isobstaclehere = true;
		floor[2][2].isobstaclehere = true;
		floor[2][5].isobstaclehere = true;
		floor[2][7].isobstaclehere = true;
		floor[4][0].isobstaclehere = true;
		floor[5][2].isobstaclehere = true;
		floor[5][5].isobstaclehere = true;
		floor[7][2].isobstaclehere = true;
		floor[7][5].isobstaclehere = true;

	} else {
		floor[2][0].isobstaclehere = true;
		floor[5][0].isobstaclehere = true;
		floor[2][2].isobstaclehere = true;
		floor[5][2].isobstaclehere = true;
		floor[7][2].isobstaclehere = true;
		floor[0][4].isobstaclehere = true;
		floor[2][5].isobstaclehere = true;
		floor[5][5].isobstaclehere = true;
		floor[2][7].isobstaclehere = true;
		floor[5][7].isobstaclehere = true;
		floor[4][4].isobstaclehere = true;
		floor[1][4].isobstaclehere = true;
		floor[6][3].isobstaclehere = true;
		floor[7][5].isobstaclehere = true;

	}




			
	enemiespossiblepossitions[0].x = 600.0 - (float)(1* 80) + (float)(7 * 80);
	enemiespossiblepossitions[0].y = 0.0 + (float)(1 * 50) + (float)(7 * 50);

	enemiespossiblepossitions[1].x = 600.0 - (float)(80.0*0) + (float)(4 * 80);
	enemiespossiblepossitions[1].y = 0.0 + (float)(50.0*0) + (float)(4 * 50);

	enemiespossiblepossitions[2].x = 600.0 - (float)(6* 80) + (float)(7 * 80);
	enemiespossiblepossitions[2].y = 0.0 + (float)(6 * 50) + (float)(7 * 50);
	
	enemiespossiblepossitions[3].x = 600.0 - (float)(4* 80) + (float)(4 * 80);
	enemiespossiblepossitions[3].y = 0.0 + (float)(4 * 50) + (float)(4 * 50);

	enemiespossiblepossitions[4].x = 600.0 - (float)(0* 80) + (float)(0 * 80);
	enemiespossiblepossitions[4].y = 0.0 + (float)(0 * 50) + (float)(0 * 50);

	enemiespossiblepossitions[5].x = 600.0 - (float)(4* 80) + (float)(7 * 80);
	enemiespossiblepossitions[5].y = 0.0 + (float)(4 * 50) + (float)(7* 50);

	enemiespossiblepossitions[6].x = 600.0 - (float)(2* 80) + (float)(3 * 80);
	enemiespossiblepossitions[6].y = 0.0 + (float)(2 * 50) + (float)(3 * 50);


	enemies[0].i = 7;
	enemies[0].j = 6;

	enemies[1].i = 4;
	enemies[1].j = 7;

	enemies[2].i = 7;
	enemies[2].j = 1;

	enemies[3].i = 4;
	enemies[3].j = 3;

	enemies[4].i = 0;
	enemies[4].j = 7;

	enemies[5].i = 7;
	enemies[5].j = 3;

	enemies[6].i = 3;
	enemies[6].j = 5;

	for (int i = 0; i < 20; i++) {

		int movem[] = {4, 3 ,3, 4, 4, 4, 2, 3, 4, 4, 3, 3, 3, 3, 4, 1, 4, 1, 4, 1};
		enemies[0].movement[i] = movem[i];
	}
	
	for (int i = 0; i < 20; i++) {

		int movem[] = {4, 3 ,3, 3 ,4, 0, 4, 4, 3, 1, 3, 3, 3, 4, 0, 2, 3, 2, 3, 2};
		enemies[1].movement[i] = movem[i];
	}

	for (int i = 0; i < 20; i++) {

		int movem[] = {4, 4 ,4, 4, 4, 4, 3, 4, 1, 4, 1, 4, 1, 4, 1, 4, 1, 4, 1, 4};
		enemies[2].movement[i] = movem[i];
	}

	for (int i = 0; i < 20; i++) {

		int movem[] = {3, 4 ,2, 4, 4, 3, 3, 4, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2};
		enemies[3].movement[i] = movem[i];
	}

	for (int i = 0; i < 20; i++) {

		int movem[] = {1, 3 ,1, 4, 3, 3, 4, 3, 1, 3, 3, 4, 3, 3, 4, 3, 2, 3, 2, 3};
		enemies[4].movement[i] = movem[i];
	}

	for (int i = 0; i < 20; i++) {

		int movem[] = {4, 3 ,3, 4, 0, 4, 4, 0, 4, 4, 4, 3, 4, 1, 4, 1, 4, 1, 4};
		enemies[5].movement[i] = movem[i];
	}

	for (int i = 0; i < 20; i++) {

		int movem[] = {3, 3 ,1, 3, 3, 4, 4, 4, 4, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3};
		enemies[6].movement[i] = movem[i];
	}



	
	



	for (int i = 0; i < 7; i++){
		int sure = enemies[i].movement[0];

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
		enemies[i].enemyx = enemiespossiblepossitions[i].x;
		enemies[i].enemyy = enemiespossiblepossitions[i].y;
		enemies[i].vetor.x = (float)enemies[i].enemyx + enemies[i].sprite.width/2;
		enemies[i].vetor.y = (float)enemies[i].enemyy + enemies[i].sprite.height/2;
		enemies[i].explosioncheck = 0;
		enemies[i].framesExplosionCounter = 0;
		enemies[i].currentExplosionFrame = 0;
		enemies[i].explosionrect.x = 0.0f;
		enemies[i].explosionrect.y = 0.0f;
		enemies[i].explosionrect.height = (float)explosion.height;
		enemies[i].explosionrect.width = (float)explosion.width/16;
		enemies[i].exists = true;
		enemies[i].direction = sure;
	}


	int positionballx;
	int positionbally;
	

	

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
				

	
				scrollingback -= 0.1f;
				scrollingmid -= 0.5f;
				scrollingfore -= 1.0f;

				if (scrollingback <= -background1.width*2) scrollingback = 0;
				if (scrollingmid <= -background2.width*2) scrollingmid = 0;
				if (scrollingfore <= -background3.width*2) scrollingfore = 0;



				Vector2 contact1 = {(float)positionballx, (float)positionbally};



				//collision
				for (int i = 0; i < 7; i++){
					bool die = false;

					die = CheckCollisionCircles(enemies[i].vetor, 25.0f, (Vector2){positionplayer.x+64,positionplayer.y+48}, 25.0f);

					if ((enemies[i].i == 1 && enemies[i].j == 0) || (enemies[i].i == 0 && enemies[i].j == 1) || die) {
						PlaySound(deadmusic);
						currentScreen = TRANSITION;
					}

					
					notelist[i].vect.x = (float)notelist[i].posnx + 32;
					notelist[i].vect.y = (float)notelist[i].posny + 32;
					enemies[i].collision = CheckCollisionCircles(notelist[i].vect, 25.0f,enemies[i].vetor, 25.0f);
					notelist[i].collision = CheckCollisionCircles(notelist[i].vect, 25.0f, enemies[i].vetor,25.0f);
					
					if (notelist[i].collision) notelist[i].turnr = 0;
					if (enemies[i].collision) enemies[i].dead = true;
					if (enemies[i].dead) {
						enemies[i].explosioncheck++;
						if (enemies[i].explosioncheck == 1){
							playerpoint++;
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
		

				//animation
				
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
							
						}

						enemies[i].explosionrect.x = (float)enemies[i].currentExplosionFrame*(float)explosion.width/16;
					}
					if(enemies[i].dead)enemies[i].framesExplosionCounter++;
				}

				if (playerjumpcounter >= (60/framesSpeed)){
					
					playerjumpcounter = 0;

					currentplayerjumpframe++;
					
					if (currentplayerjumpframe > 15){
						if (!TimerDone(&turntimer)) currentplayerjumpframe = 0;
						else currentplayerjumpframe = 15;
					}
					if (dir == 1) playerjumprectdown.x = (float)currentplayerjumpframe*(float)playerjumpdown.width/16;
					if (dir == 2) playerjumprectright.x = (float)currentplayerjumpframe*(float)playerjumpright.width/16;
					if (dir == 3) playerjumprectleft.x = (float)currentplayerjumpframe*(float)playerjumpleft.width/16;
					if (dir == 4) playerjumprectup.x = (float)currentplayerjumpframe*(float)playerjumpup.width/16;
				}
				if (playermovemet) playerjumpcounter++;

				


				
				//remember to update player movement 
				if (TimerDone(&turntimer) && (currentplayerjumpframe == 0 || currentplayerjumpframe == 15)){
					if(IsKeyPressed(KEY_DOWN)) {

						
						if (dir == 1 && (playeri < 7) && !(floor[playeri+1][playerj].isobstaclehere)) {	
							playermovemet = 1;
							notebeingshot = 1;
							
							turn++;
							StartTimer(&turntimer,turnduration);
							turncom = turn;
							StartTimer(&notetimer, notetimerduration);
							shootmove(notelist);
							playeri++;

							for(int i = 0; i < 7; i++) enemies[i].direction = enemies[i].movement[enemymovementcounter];
							enemymovementcounter++;
							
						} else {
							dir = 1;
						}
						
					}
					if(IsKeyPressed(KEY_UP)) {

						if (dir == 4 && playeri > 0 && !(floor[playeri-1][playerj].isobstaclehere)){
							playermovemet = 1;
							notebeingshot = 1;
							turn++;
							StartTimer(&turntimer,turnduration);
							turncom = turn;
							shootmove(notelist);
							playeri--;
							
							for(int i = 0; i < 7; i++) enemies[i].direction = enemies[i].movement[enemymovementcounter];
							enemymovementcounter++;

						} else {
							dir = 4;
						}
					}
					if(IsKeyPressed(KEY_RIGHT)) {
						
						if (dir == 2 && playerj < 7 && !(floor[playeri][playerj+1].isobstaclehere)){
							playermovemet = 1;
							notebeingshot = 1;
							turn++;
							StartTimer(&turntimer,turnduration);
							turncom = turn;
							shootmove(notelist);
							playerj++;

							for(int i = 0; i < 7; i++) enemies[i].direction = enemies[i].movement[enemymovementcounter];
							enemymovementcounter++;
							
						} else {
							dir = 2;
						}
					}
					if(IsKeyPressed(KEY_LEFT)) {

						if (dir == 3 && playerj > 0 && !(floor[playeri][playerj-1].isobstaclehere)){
							playermovemet = 1;
							notebeingshot = 1;
							turn++;
							StartTimer(&turntimer,turnduration);
							turncom = turn;
							shootmove(notelist);
							playerj--;

							for(int i = 0; i < 7; i++) enemies[i].direction = enemies[i].movement[enemymovementcounter];
							enemymovementcounter++;
							
						} else {
							dir = 3;
						}
					}
					
					if(IsKeyPressed(KEY_SPACE) && turn != 0){
						
						notebeingshot = 0;
						notemovement = 1;
						playermovemet = 0;
						
						
						if(notelist[notecheck].turnr == 0 || (notelist[notecheck].posnx > screenwidth) || (notelist[notecheck].posny > screenheight) || (notelist[notecheck].posnx < 0) || (notelist[notecheck].posny < 0)){ //turn - notelist[notecheck].turnr >= 3
							turn++;
							notelist[notecheck].direct = dir;
							notelist[notecheck].posnx = positionplayer.x;
							notelist[notecheck].posny = positionplayer.y;
							if(dir == 1) {
								notelist[notecheck].posny += 140;
								notelist[notecheck].posnx += 105;
							}
							if(dir == 2) {
								notelist[notecheck].posny += 30;
								notelist[notecheck].posnx += 115;
							}
							if(dir == 3) {
								notelist[notecheck].posny += 140;
								notelist[notecheck].posnx -= 50;
							}
							if(dir == 4) {
								notelist[notecheck].posny += 40;
								notelist[notecheck].posnx -= 50;
							}
							notelist[notecheck].turnr = turn;
							
							PlaySound(notelist[notecheck].notesound);
							
							StartTimer(&turntimer,turnduration);
							shootmove(notelist); 
							turncom = turn;

							for(int i = 0; i < 7; i++) enemies[i].direction = enemies[i].movement[enemymovementcounter];
							enemymovementcounter++;
						}

					}
					
				}
				if(IsKeyPressed(KEY_ONE)) {
						notecheck++;
						if (notecheck > 6) notecheck = 0;
				}

	

				if (dir == 2){

					positionballx = (int)positionplayer.x + 134;
					positionbally = (int)positionplayer.y + 75; 
				} 

				if (dir == 4){
					positionballx = (int)positionplayer.x - 10;
					positionbally = (int)positionplayer.y + 60; 
				}

				if (dir == 1){
					positionballx = (int)positionplayer.x + 144;
					positionbally = (int)positionplayer.y + 170; 
				}

				if (dir == 3) {
					positionballx = (int)positionplayer.x - 10;
					positionbally = (int)positionplayer.y + 165; 
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



				//smooth movement of the notes
				if (!TimerDone(&turntimer) && notemovement){
				
					shootmove(notelist);


				}

				if (!TimerDone(&turntimer) && enemymove){

				
					enemymovement(enemies);
						
					

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


				mousebutn = GetMousePosition();

				buttonmenuactive = false;

				buttonexitactive = false;


				


				if (CheckCollisionPointRec(mousebutn, buttonexitboundnew)) {
					if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) buttonexitstate = 2;
					else buttonexitstate = 1;

					if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) buttonexitactive = true;
				} else buttonexitstate = 0;

				buttonexitsourcenew.x = (float)(buttonexitstate*(exitbutton.width/3));

				if (buttonexitactive)exitWindow = true;

				if (CheckCollisionPointRec(mousebutn, buttonmenuboundnew)) {
					if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) buttonmenustate = 2;
					else buttonmenustate = 1;

					if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) buttonmenuactive = true;
				} else buttonmenustate = 0;

				buttonmenusourcenew.x = (float)(buttonmenustate*(menubutton.width/3));

				if (buttonmenuactive) {
					currentScreen = STARTSCREEN;
					restart = 1;
				}

			}break;

			case TRANSITION:{
				

				framesCounter++;

				if (framesCounter > 120){
					currentScreen = DEAD;
					framecounter = 0;
				}
			}break;

			case CREDITS:{
				UpdateMusicStream(startmusic);
				if (IsKeyPressed(KEY_ESCAPE)) currentScreen = STARTSCREEN;
			}break;

			case HIGHSCORE:{

			}break;
			default: break;
		}
		
		
		

		BeginDrawing();


		ClearBackground(GetColor(0x052c46ff));


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


				DrawTextureEx(background1, (Vector2){ scrollingback, 20 }, 0.0f, 2.0f, WHITE);
				DrawTextureEx(background1, (Vector2){ background1.width + scrollingback, 20 }, 0.0f, 2.0f, WHITE);

				DrawTextureEx(background2, (Vector2){ scrollingmid, 20 }, 0.0f, 2.0f, WHITE);
				DrawTextureEx(background2, (Vector2){ background2.width + scrollingmid, 20 }, 0.0f, 2.0f, WHITE);


				DrawTextureEx(background3, (Vector2){ scrollingfore, 70 }, 0.0f, 2.0f, WHITE);
				DrawTextureEx(background3, (Vector2){ background3.width + scrollingfore, 70 }, 0.0f, 2.0f, WHITE);



				//create if statements so things only get drawn when i need them
				for (int i = 0; i < 7; i++){
					notelist[i].vect.x = notelist[i].posnx;
					notelist[i].vect.y = notelist[i].posny;
				}
				
				DrawTexture(floortest1, 20, 40, WHITE);
				DrawTexture(exitladder, exitladderx, exitladdery, WHITE);
				DrawTexture(playersoul, soulposx, soulposy, WHITE);

				for (int i = 0; i < 8; i++){
					for (int j = 0; j < 8; j++){
						if(floor[i][j].isobstaclehere) DrawCircle((int)floor[i][j].position.x,(int)floor[i][j].position.y, 20, RED);
					}
				}


				for (int i = 0; i < 7; i++){
					if (notelist[i].turnr > 0  && !notelist[i].collision) DrawTextureRec(notelist[i].sprite, notelist[i].rect,notelist[i].vect, WHITE);
					//if (notelist[i].turnr > 0) DrawCircle(notelist[i].vect.x + 32,notelist[i].vect.y + 32, 20.0f, BLUE);
				}
				

			
				//temporary mouse text so i can figure out positions
				DrawTextEx(GetFontDefault(), TextFormat("[%i, %i]", GetMouseX(), GetMouseY()),
						Vector2Add(GetMousePosition(), (Vector2){ -44, -24 }), 20, 2, BLACK);


				for (int i = 0; i < 7; i++){
					
					if(!enemies[i].dead && enemies[i].exists)DrawTexture(enemies[i].sprite,enemies[i].enemyx,enemies[i].enemyy,WHITE);
					if (enemies[i].dead)DrawTextureRec(explosion, enemies[i].explosionrect, enemies[i].explosionvect , WHITE);
					
					if (notelist[i].collision) DrawText("contact", 100, 100, 100, BLACK);
				}

				//DrawTexture(obstacle, 409, 395, WHITE);


				if (dir == 1 && !playermovemet) DrawTextureRec(playersprite1, frameRecplayer, positionplayer, WHITE);
				if (dir == 3 && !playermovemet) DrawTextureRec(playersprite2, frameRecplayer, positionplayer, WHITE);
				if (dir == 2 && !playermovemet) DrawTexture(playersprite3, positionplayer.x, positionplayer.y, WHITE);
				if (dir == 4 && !playermovemet) DrawTexture(playersprite4, positionplayer.x, positionplayer.y, WHITE);
				if (dir == 1 && playermovemet) DrawTextureRec(playerjumpdown, playerjumprectdown, positionplayer, WHITE);
				if (dir == 4 && playermovemet) DrawTextureRec(playerjumpup, playerjumprectup, positionplayer, WHITE);
				if (dir == 2 && playermovemet) DrawTextureRec(playerjumpright, playerjumprectright, positionplayer, WHITE);
				if (dir == 3 && playermovemet) DrawTextureRec(playerjumpleft, playerjumprectleft, positionplayer, WHITE);


				if (collisionball1) DrawText("1", 100, 100, 100, BLACK);
				if (collisionball2) DrawText("2", 100, 100, 100, BLACK);
				if (collisionball3) DrawText("3", 100, 100, 100, BLACK);
				if (collisionball4) DrawText("4", 100, 100, 100, BLACK);

				DrawCircle(positionballx,positionbally, 20, GRAY);

				
			
				

				
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
				DrawText("Game made entirely by Vinicius Tenorio", 380, 400, 30, RED);
				DrawText("This project was made without any help of A.I", 0, 880, 5, BLACK);
				DrawText("Press exit to go back to the main menu", 380, 450, 20, WHITE );
				
			}break;

			case HIGHSCORE:{

			}break;

			case DEAD:{

				DrawRectangle(0, 0, screenwidth, screenheight,WHITE);

				if (!IsSoundPlaying(deadmusic)){
					DrawText("You lose", screenwidth/3,screenheight/3,20, BLACK);
					DrawText(TextFormat("you killed a total of %d enemies",playerpoint),screenwidth/2, screenheight/2,20,BLACK);

					DrawTextureRec(menubutton, buttonmenusourcenew, (Vector2){ buttonmenuboundnew.x, buttonexitboundnew.y}, WHITE);

					DrawTextureRec(exitbutton, buttonexitsourcenew, (Vector2){ buttonexitboundnew.x, buttonexitboundnew.y}, WHITE);
				}



			}break;

			case TRANSITION:{

				DrawTextureEx(background1, (Vector2){ scrollingback, 20 }, 0.0f, 2.0f, WHITE);
				DrawTextureEx(background1, (Vector2){ background1.width + scrollingback, 20 }, 0.0f, 2.0f, WHITE);

				DrawTextureEx(background2, (Vector2){ scrollingmid, 20 }, 0.0f, 2.0f, WHITE);
				DrawTextureEx(background2, (Vector2){ background2.width + scrollingmid, 20 }, 0.0f, 2.0f, WHITE);


				DrawTextureEx(background3, (Vector2){ scrollingfore, 70 }, 0.0f, 2.0f, WHITE);
				DrawTextureEx(background3, (Vector2){ background3.width + scrollingfore, 70 }, 0.0f, 2.0f, WHITE);



				//create if statements so things only get drawn when i need them
				for (int i = 0; i < 7; i++){
					notelist[i].vect.x = notelist[i].posnx;
					notelist[i].vect.y = notelist[i].posny;
				}
				
				DrawTexture(floortest1, 20, 40, WHITE);
				DrawTexture(exitladder, exitladderx, exitladdery, WHITE);
				DrawTexture(playersoul, soulposx, soulposy, WHITE);

				for (int i = 0; i < 8; i++){
					for (int j = 0; j < 8; j++){
						if(floor[i][j].isobstaclehere) DrawCircle((int)floor[i][j].position.x,(int)floor[i][j].position.y, 20, RED);
					}
				}


				for (int i = 0; i < 7; i++){
					if (notelist[i].turnr > 0  && !notelist[i].collision) DrawTextureRec(notelist[i].sprite, notelist[i].rect,notelist[i].vect, WHITE);
					//if (notelist[i].turnr > 0) DrawCircle(notelist[i].vect.x + 32,notelist[i].vect.y + 32, 20.0f, BLUE);
				}
				

			
				//temporary mouse text so i can figure out positions
				DrawTextEx(GetFontDefault(), TextFormat("[%i, %i]", GetMouseX(), GetMouseY()),
						Vector2Add(GetMousePosition(), (Vector2){ -44, -24 }), 20, 2, BLACK);


				for (int i = 0; i < 7; i++){
					
					if(!enemies[i].dead && enemies[i].exists)DrawTexture(enemies[i].sprite,enemies[i].enemyx,enemies[i].enemyy,WHITE);
					if (enemies[i].dead)DrawTextureRec(explosion, enemies[i].explosionrect, enemies[i].explosionvect , WHITE);
					
					if (notelist[i].collision) DrawText("contact", 100, 100, 100, BLACK);
				}

				//DrawTexture(obstacle, 409, 395, WHITE);


				if (dir == 1 && !playermovemet) DrawTextureRec(playersprite1, frameRecplayer, positionplayer, WHITE);
				if (dir == 3 && !playermovemet) DrawTextureRec(playersprite2, frameRecplayer, positionplayer, WHITE);
				if (dir == 2 && !playermovemet) DrawTexture(playersprite3, positionplayer.x, positionplayer.y, WHITE);
				if (dir == 4 && !playermovemet) DrawTexture(playersprite4, positionplayer.x, positionplayer.y, WHITE);
				if (dir == 1 && playermovemet) DrawTextureRec(playerjumpdown, playerjumprectdown, positionplayer, WHITE);
				if (dir == 4 && playermovemet) DrawTextureRec(playerjumpup, playerjumprectup, positionplayer, WHITE);
				if (dir == 2 && playermovemet) DrawTextureRec(playerjumpright, playerjumprectright, positionplayer, WHITE);
				if (dir == 3 && playermovemet) DrawTextureRec(playerjumpleft, playerjumprectleft, positionplayer, WHITE);


				if (collisionball1) DrawText("1", 100, 100, 100, BLACK);
				if (collisionball2) DrawText("2", 100, 100, 100, BLACK);
				if (collisionball3) DrawText("3", 100, 100, 100, BLACK);
				if (collisionball4) DrawText("4", 100, 100, 100, BLACK);

				DrawCircle(positionballx,positionbally, 20, GRAY);

				
			
				

				
				for (int i = 0; i < 7; i++){
					if (i == notecheck) DrawRectangle(30 + 71*i, 800, 70, 70, RED);
					DrawTexture(notenoan[i], 30 + 71*i, 803, WHITE);
					DrawRectangleLines(30 + 71*i, 800, 70, 70, MAROON);
				}



				DrawRectangle(0, 0, screenwidth, screenheight,CLITERAL(Color){255,255,255, framesCounter*2});

			}break;
			default: break;
		}

		
		
		
		EndDrawing();
	}

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
	UnloadTexture(background1);
	UnloadTexture(background2);
	UnloadTexture(background3);
	for (int j = 0; j < 8; j++){
		free(floor[j]); 
	}
	free(floor);



	CloseAudioDevice();
	CloseWindow();
	return 0;
}
