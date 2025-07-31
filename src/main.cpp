#include "game.h"

SETTINGS

int main(int, char*[])
{

    foreach ( ConfigFlags _flags  : Settings::FLAGS ) {
        SetConfigFlags(_flags);
    }
    InitWindow(win_w, win_h, "");
    SetTargetFPS(FPS);

    SETUP();
    
    while (!WindowShouldClose())
    {
        SetWindowTitle(std::to_string(FPS).c_str());
        UPDATE();
        
        //BeginDrawing();
        //    ClearBackground(win_clr);
            RENDER();
        //EndDrawing();
    }

    CLEAN();
    
    CloseWindow();

}