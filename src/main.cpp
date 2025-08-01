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
    

    uint16 frames = 0u;
    f64 timer = GetTime();
    while (!WindowShouldClose())
    {
        ++frames;
        f64 now = GetTime();
        if (now - timer >= 1.0) {
            SetWindowTitle(std::to_string(frames).c_str());
            frames = 0u;
            timer = now;
        }

        UPDATE();
        
        //BeginDrawing();
        //    ClearBackground(win_clr);
        //    RENDER();
        //EndDrawing();
    }

    CLEAN();
    
    CloseWindow();

}