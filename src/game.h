#pragma once
#include "utils.h"


#define SETTINGS

inline namespace Settings
{
    constexpr inline uint16 win_w   =  1500u;
    constexpr inline uint16 win_h   =  840u;
    constexpr inline Color  win_clr =  {0u, 0u, 0u, 255u};

    constexpr inline uint16  FPS    =  60u;

    constexpr inline std::array<ConfigFlags, 15> FLAGS
    {
        FLAG_WINDOW_RESIZABLE,

    };
};


extern Font font;



struct GFX
{
    void write(const char *text, Vector2 pos, Color color = {WHITE},
        uint fsize = 24, f spacing = 0.5f) const{

        //    BeginDrawing();
        DrawTextEx(font, text, pos, fsize, spacing, color);
        //    EndDrawing();
    }

    void write(const char text, Vector2 pos, Color color = {WHITE},
        uint fsize = 40, f spacing = 0.5f) const{

        //    BeginDrawing();
        DrawTextEx(font, tostr(text).c_str(), pos, fsize, spacing, color);
        //    EndDrawing();
    }



    void drawRect(Rectangle rec, Color color, f radius=0.f) const{
        DrawRectangleRounded(rec, radius, 10000, color);
    }

    void drawCircle(Vector2 pos, uint radius, Color color) const{
        DrawCircle(pos.x, pos.y, radius, color);
    }
};

constexpr GFX gfx;



struct Uvec2
{

    // overall size
    uint16 size = sizeof(uint)*2;

    uint x;
    uint y;
};
