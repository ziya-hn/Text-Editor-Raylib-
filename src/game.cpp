#pragma GCC diagnostic ignored "-Wunused-variable"

#include "game.h"

#if __cplusplus >= 202302  /// Compiler should be flagged as at least `-std=c++23`


Font font;

Module G  ///  Global/Editor, short name convention
{
    constexpr Color bgcolor = { 9, 16, 30, 255 };

    constexpr uint xofs =  80u;
    constexpr uint yofs = 120u;

    constexpr uint scene_width  = win_w - 2*xofs;
    constexpr uint scene_height = win_h - 2*yofs;

    Rectangle rec = { xofs, yofs, scene_width, scene_height };

    Strvec map;


    void start(void) {
        
    }
    
    void loop(void)
    {
        using Module z;

        if     ( hadKeyPressing(k_right) )  log "`right` -pressed\n";
        else if ( hadKeyPressing(k_left)  )  log "`left` -pressed\n";
        else if ( hadKeyPressing(k_up)    )  log "`up` -pressed\n"  ;
        else if ( hadKeyPressing(k_down)  )  log "`down` -pressed\n";
        else;

    }

    void draw(void)
    {
        gfx.drawRect(rec, bgcolor, 0);
    }
};



struct Cursor
{
    Color color =  { 200, 200, 200, 200 };
    constexpr static f  radius   =  { 0.5f };

    constexpr static const char* font =  "Assets\\CascadiaCode-Bold.ttf";
    uint fontsize     =  32u;
    Color font_color  =  { 240, 240, 240, 255 };


    //  Cursor draw style
    enum class CursorDrawStyle  {
        line,
        rectangle,
        capsule,
    };
    CursorDrawStyle CURRENT;

    
    //  RUNTIME SPECIAL ->

    Vector2  pos  =  { (f)G::xofs, (f)G::yofs };

    Vector2 index =  { 0, 0 };

    f G_n_Cursor_scale_X$ratio =  42;
    f G_n_Cursor_scale_Y$ratio =  12;
    uint width  =  G::scene_width / G_n_Cursor_scale_X$ratio;
    uint height =  G::scene_height / G_n_Cursor_scale_Y$ratio;

    f cursor_ln_w_rate =  0.05f;

    Rectangle rec = { pos.x, pos.y, (f)width, (f)height };

    uchar current_char;


    void right(void) {
        this->pos.x += width;
    }

    void left(void) {
        this->pos.x -= width;
    }

    void up(void) {
        this->pos.y -= height;
    }

    void down(void) {
        this->pos.y += height;
    }


    void start(void) {

    }

    void loop(void)
    {
        rec.x = pos.x;
        rec.y = pos.y;

        z::limit<f>( pos.x, G::xofs ,  G::xofs + G::scene_width - this->width );
        z::limit<f>(pos.y, G::yofs ,  G::yofs + G::scene_height - this->height);


        //  Handle draw-style with numpad[0]

        using CDS = CursorDrawStyle;

        if (CURRENT == CDS::capsule  && IsKeyPressed(KEY_KP_0)) {
            CURRENT =  CDS::line;
        }
        else if (CURRENT == CDS::line  && IsKeyPressed(KEY_KP_0)) {
            CURRENT =  CDS::rectangle;
        }
        else if (CURRENT == CDS::rectangle  && IsKeyPressed(KEY_KP_0)) {
            CURRENT =  CDS::capsule;
        }
        else;  // Bəzəyə qoydum bunu 🥀

    }


    void draw(void)
    { 
        using CDS = CursorDrawStyle;

        switch (CURRENT)
        {
            case CDS::capsule:  gfx.drawRect(this->rec, this->color, this->radius); break;

            case CDS::line :
            {
                using Rc = Rectangle;
                gfx.drawRect
                (
                    Rc {pos.x + (width * (1-cursor_ln_w_rate)) - width, pos.y, width * cursor_ln_w_rate, (f)height} ,
                    this->color ,  this->radius
                );
            }  break;

            case CDS::rectangle:  gfx.drawRect(this->rec, this->color, 0); break;

        }

    }
};
Cursor cursor;



// Convert position(pixels) to index in Vector2
Vector2 to_i(f x, f y) {
    f xi = (x - G::xofs) / cursor.width;
    f yi = (y - G::yofs) / cursor.height;

    return { xi, yi };
}

uint xtoi(f x) {
    return  { ((uint)x - G::xofs) / cursor.width };
}

uint ytoi(f y) {
    return  { ((uint)y - G::yofs) / cursor.height };
}




void SETUP(void)
{
    font = LoadFontEx(cursor.font, 32, nullptr, 0);

    G::start();
    cursor.start();
}



 void UPDATE(void)
{
    using Module z;

    bool righting = IsKeyPressed(k_right);
    bool  lefting = IsKeyPressed(KEY_LEFT);
    bool    up    = IsKeyPressed(k_up);
    bool   down   = IsKeyPressed(k_down);

    bool can_go_right = cursor.pos.x < (win_w - G::xofs - cursor.width);
    bool can_go_left  = cursor.pos.x > G::xofs;
    bool can_go_down  = cursor.pos.y < (win_h - G::yofs - cursor.height);
    bool can_go_up    = cursor.pos.y > G::yofs;

    uchar pressed_char = GetCharPressed();

    G::loop();
    cursor.loop();



    // Cursor movement

    if (can_go_right && (hadKeyPressing(k_right))) {
        cursor.right();
    }
    else if ( !can_go_right  &&  can_go_down  &&  (hadKeyPressing(k_right))) {
        cursor.pos.x =  G::xofs;
        cursor.down();
    }

    else if (can_go_left && (hadKeyPressing(k_left))) {
        cursor.left();
    }
    else if ( !can_go_left  &&  can_go_up  &&  (hadKeyPressing(k_left)) ) {
        cursor.pos.x =  G::xofs + G::scene_width - cursor.width;
        cursor.up();
    }

    else if (can_go_down && ( hadKeyPressing(k_down) )) {
        cursor.down();
    }
    else if (can_go_up && ( hadKeyPressing(k_up) )) {
        cursor.up();
    }

    //////////
    DRAWER();
    //////////

    G::draw();
    cursor.draw();


    // Normal writing -(not rendering)
    if (z::printable(pressed_char)) {
        uint x_idx = xtoi(cursor.pos.x);
        uint y_idx = ytoi(cursor.pos.y);

        G::map(x_idx, y_idx) = pressed_char;

        if (can_go_right) {
            cursor.right();
        }
    }

    if (hadKeyPressing(k_enter)) {
        if (can_go_down) {
            cursor.pos.x =  G::xofs;
            cursor.down();
        }
    }

    // Backspace logic
    if (hadKeyPressing(k_backspace)) {
        if (can_go_left) {
            cursor.left();
            uint x_idx = xtoi(cursor.pos.x);
            uint y_idx = ytoi(cursor.pos.y);
            G::map(x_idx, y_idx) = '\0';
        }
        else if (can_go_up)  {
            //  if cursor is in beginning of line, goto up,
            cursor.up();
            //  and if next char in $map is not null, then advance
            while ( G::map((cursor.pos.x), (cursor.pos.y))  != '\0' ) {
                cursor.right();
            }
        }
    }

    
    // Render all characters at their *grid* positions
    for (uint i = 0; i < G::map.getHeight(); ++i) {
        for (uint j = 0; j < G::map.getWidth(); ++j) {
            char ch = G::map(j, i);

            if (ch != '\0') {
                Vector2 char_pos = {
                    (float)(G::xofs + j * cursor.width),
                    (float)(G::yofs + i * cursor.height)
                };

                gfx.write(ch, char_pos);
            }
        }
    }

    EndDrawing();
}


void RENDER(void) {}


void CLEAN(void)
{
    UnloadFont(font);
}


#endif // __cplusplus >= 202302
