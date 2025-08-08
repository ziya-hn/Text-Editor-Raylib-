#pragma once
#include "utils.h"


#define SETTINGS

inline namespace Settings
{
    constexpr inline uint16 init_win_w   =  1500u;
    constexpr inline uint16 init_win_h   =  840u;

    // Define as inline variables to avoid multiple definition errors
    inline constinit uint16 win_w = init_win_w;
    inline constinit uint16 win_h = init_win_h;

    constexpr inline Color  win_clr =  {0u, 0u, 0u, 255u};

    constexpr inline uint16  FPS    =  60u;

    constexpr inline std::array<ConfigFlags, 15> FLAGS
    {
        FLAG_WINDOW_RESIZABLE,

    };
};


extern Font font;


enum K_Keys
{
    //  Movement-Keys
    k_right =  KEY_RIGHT,
    k_left  =  KEY_LEFT,
    k_up    =  KEY_UP,
    k_down  =  KEY_DOWN,

    //  Control-Keys
    k_enter     =  KEY_ENTER,
    k_esc       =  KEY_ESCAPE,
    k_space     =  KEY_SPACE,  // yeah, its not control-key
    k_tab       =  KEY_TAB,
    k_backspace =  KEY_BACKSPACE,
    Kdelete  =  KEY_DELETE,



    //  Numpad-Keys
    k_numpad_0 =  KEY_KP_0,
    k_numpad_1 =  KEY_KP_1,
    k_numpad_2 =  KEY_KP_2,
    k_numpad_3 =  KEY_KP_3,
    k_numpad_4 =  KEY_KP_4,
    k_numpad_5 =  KEY_KP_5,
    k_numpad_6 =  KEY_KP_6,
    k_numpad_7 =  KEY_KP_7,
    k_numpad_8 =  KEY_KP_8,
    k_numpad_9 =  KEY_KP_9,


};


struct GFX
{
    void write(const char *text, Vector2 pos, Color color = {WHITE},
        uint fsize = 24, f spacing = 0.5f) const {

        DrawTextEx(font, text, pos, fsize, spacing, color);
    }

    void write(const char text, Vector2 pos, Color color = {WHITE},
        uint fsize = 40, f spacing = 0.5f) const {

        DrawTextEx(font, tostr(text).c_str(), pos, fsize, spacing, color);
    }



    void drawRect(Rectangle rec, Color color, f radius=0.f) const {
        DrawRectangleRounded(rec, radius, 10000, color);
    }

    void drawCircle(Vector2 pos, uint radius, Color color) const {
        DrawCircle(pos.x, pos.y, radius, color);
    }
};

constexpr GFX gfx;


Module z
{
    inline bool hadKeyPressing(KeyboardKey _key) {
        return  { IsKeyPressed(_key) || IsKeyPressedRepeat(_key) };
    }
    // overload of @fn hadKeyPressing
    inline bool hadKeyPressing(K_Keys _key) {
    return  { IsKeyPressed(_key) || IsKeyPressedRepeat(_key) };
    }
}

