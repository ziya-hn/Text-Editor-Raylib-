#pragma GCC diagnostic ignored "-Wunused-variable"

#include "game.h"



Font font;



Module G
{
    constexpr Color bgcolor = { 9, 16, 30, 255 };

    constexpr uint xofs =  80u;
    constexpr uint yofs = 120u;

    constexpr uint width  = win_w - 2*xofs;
    constexpr uint height = win_h - 2*yofs;

    Rectangle rec = { xofs, yofs, width, height };

    Strvec map;


    void start(void) {
        
    }
    
    void loop(void)
    {
        if (IsKeyPressedRepeat(KEY_RIGHT))     log "`right` -pressed\n";
        else if (IsKeyPressedRepeat(KEY_LEFT)) log "`left` -pressed\n" ;
        else if (IsKeyPressedRepeat(KEY_UP))   log "`up` -pressed\n" ;
        else if (IsKeyPressedRepeat(KEY_DOWN)) log "`down` -pressed\n" ;
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
    constexpr static f  radius   =  { 0.25 };

    constexpr static const char* font =  "Assets\\CascadiaCode-Bold.ttf";
    uint fontsize     =  32u;
    Color font_color  =  { 240, 240, 240 };


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

    uint width  =  32u;
    uint height =  50u;

    f cursor_ln_w_rate =  0.9f;

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
        else;  // Bəzəyə qoydum bunu 😔✌️

    }


    void draw(void)
    { 
        using CDS = CursorDrawStyle;

        switch (CURRENT)
        {
            case CDS::capsule:  gfx.drawRect(this->rec, this->color, this->radius); break;

            case CDS::line :
            {
                using _Rct = Rectangle;
                gfx.drawRect
                (
            _Rct{pos.x + (width * cursor_ln_w_rate), pos.y, width * cursor_ln_w_rate, (f)height},
            this->color, this->radius
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
    bool righting = IsKeyPressed(KEY_RIGHT);
    bool  lefting = IsKeyPressed(KEY_LEFT);
    bool    up    = IsKeyPressed(KEY_UP);
    bool   down   = IsKeyPressed(KEY_DOWN);

    bool can_go_right = cursor.pos.x < (win_w - G::xofs - cursor.width);
    bool can_go_left  = cursor.pos.x > G::xofs;
    bool can_go_down  = cursor.pos.y < (win_h - G::yofs - cursor.height);
    bool can_go_up    = cursor.pos.y > G::yofs;

    uchar pressed_char = GetCharPressed();

    G::loop();
    cursor.loop();

    // Cursor movement
    if (can_go_right && (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT))) {
        cursor.right();
    }
    else if (can_go_left && (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT))) {
        cursor.left();
    }
    else if (can_go_down && (IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN))) {
        cursor.down();
    }
    else if (can_go_up && (IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP))) {
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

    if ((IsKeyPressed(KEY_ENTER) || IsKeyPressedRepeat(KEY_ENTER))) {
        if (can_go_down) {
            cursor.pos.x =  G::xofs;
            cursor.down();
        }
    }

    // Backspace logic
    if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))) {
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
