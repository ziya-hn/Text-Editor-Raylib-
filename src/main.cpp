#include "game.h"

SETTINGS

int main(int, char*[])
{

    log "Main function started!\n\n";
    foreach ( ConfigFlags _flags  : Settings::FLAGS ) {
        SetConfigFlags(_flags);
    }
    InitWindow(win_w, win_h, "");
    log "Window initialized!\n\n\n";
    SetTargetFPS(FPS);

    SETUP();
    log "SETUP complete!\n\n\n";

    uint16 frames = 0u;
    f64 timer = GetTime();

    log "Game loop started!\n\n\n\n";
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

    log "Clean-Up started\n\n\n";
    CLEAN();
    
    log "Cleaned-up ";
    CloseWindow();
    log "Window Closed!\n\n";

}