#include "raylib.h"

void buttonswork(Vector2 mousebutn, Rectangle buttonbound, int buttonstate, bool buttonactive, Rectangle buttonsource, Texture2D buttonsprite, int currentScreen){
    if (CheckCollisionPointRec(mousebutn, buttonbound)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) buttonstate = 2;
        else buttonstate = 1;

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) buttonactive = true;
    } else buttonstate = 0;

    buttonsource.x = (float)(buttonstate*(buttonsprite.width/3));

    if (buttonactive) currentScreen = 6;
}