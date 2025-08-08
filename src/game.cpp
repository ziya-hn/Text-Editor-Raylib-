#pragma GCC diagnostic ignored "-Wunused-variable"

#include "game.h"

#if __cplusplus >= 202302  /// Compiler should be flagged as at least `-std=c++23`


Font font;


uint xtoi(f x);
uint ytoi(f y);

Module G  ///  Global/Editor, short name convention
{
    // Global-Scale-Variables
    f GX = 1.f;
    f GY = 1.f;
    f GS = 1.f;

    constexpr Color bgcolor = { 9, 16, 30, 255 };

    f xofs =  80u;
    f yofs = 120u;

    f scene_width  = win_w - 2*xofs;
    f scene_height = win_h - 2*yofs;

    Rectangle rec = { xofs, yofs, scene_width, scene_height };

    Strvec map;


    constinit uint tabSize = 4u;


    void start(void) {
        win_w = Settings::init_win_w;
        win_h = Settings::init_win_h;
    }
    
    void loop(void)
    {
        using Module z;

        Settings::win_w = GetScreenWidth();
        Settings::win_h = GetScreenHeight();


        if     ( hadKeyPressing(k_right) )  log "`right` -pressed\n";
        else if( hadKeyPressing(k_left)  )  log "`left` -pressed\n";
        else if( hadKeyPressing(k_up)    )  log "`up` -pressed\n"  ;
        else if( hadKeyPressing(k_down)  )  log "`down` -pressed\n";
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


    ///  Cursor draw style
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

    ///////////////////////////

    void rightEx(void) {
        if (pos.x < (win_w - G::xofs - width)) {
            this->pos.x += width;
        }
    }

    void leftEx(void) {
        if (pos.x > G::xofs) {
            this->pos.x += width;
        }
    }

    void upEx(void) {
        if (pos.y < (win_h - G::yofs - height)) {
            this->pos.y += height;
        }
    }

    void downEx(void) {
        if (pos.y > G::yofs) {
            this->pos.y += height;
        }
    }

    /////////////////////////////

    template <char _default_char = '\0'>
    void delete_char(f _x, f _y) {
        G::map.operator()(xtoi(_x), ytoi(_y)) = _default_char;
    }
    //  Overload
    template <char _default_char = '\0'>
    // delete overlapping char
    void delete_char() {
        G::map.operator()(xtoi(this->pos.x), ytoi(this->pos.y)) = _default_char;
    }


    void writexy(const char _c, Vector2 _pos) {
        G::map.data[ytoi(_pos.y)].insert(xtoi(_pos.x), 1, _c);
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
    return  { uint(x - G::xofs) / cursor.width };
}

uint ytoi(f y) {
    return  { uint(y - G::yofs) / cursor.height };
}




void SETUP(void)
{
    font = LoadFontEx(cursor.font, 32, nullptr, 0);

    G::start();
    cursor.start();
}


/**********************************************/\
/**********************************************/\
/**********************************************/\
/**********************************************/\
/**********************************************/\

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

    uchar current_char = GetCharPressed();

    G::loop();
    cursor.loop();



    // Cursor movement

    if (can_go_right && (hadKeyPressing(k_right))) {
        cursor.right();
    }
    else if ( !can_go_right  &&  can_go_down  &&  (hadKeyPressing(k_right))) {
        cursor.down();
        cursor.pos.x =  G::xofs;
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

    else;


    //////////
    DRAWER();
    //////////

    G::draw();
    cursor.draw();


    // Writing Logic -(not rendering)
    if (z::printable(current_char)) {

        if (can_go_right) {
            cursor.writexy(current_char, cursor.pos);
            cursor.right();
        }
        else if (!can_go_right && can_go_down) {
            cursor.down();
            cursor.pos.x =  G::xofs;
            cursor.writexy(current_char, cursor.pos);
        }
    }


    // Skip line with Enter
    if (hadKeyPressing(k_enter)) {
        if (can_go_down) {
            cursor.pos.x =  G::xofs;
            cursor.down();
        }
    }

    // Tab pressing
    if (hadKeyPressing(k_tab)) {
        if (can_go_right) {
            uint _times = G::tabSize;
            while(_times && can_go_right) { --_times; cursor.right(); }
        }
    }

    // Backspace logic
    if (hadKeyPressing(k_backspace)) {
        // get current grid positions
        uint x_i = xtoi(cursor.pos.x);
        uint y_i = ytoi(cursor.pos.y);

        // normal case: not at line-start -> move left and delete
        if (x_i > 0) {
            cursor.left();
            cursor.delete_char(); // deletes char at new cursor cell
        }
        // at line-start and there is a previous line
        else if (y_i > 0) {
            uint prev_y = y_i - 1;

            // find last non-null column on previous line
            int last_idx = -1;
            for (uint col = 0; col < G::map.getWidth(); ++col) {
                if (G::map(col, prev_y) != '\0') last_idx = (int)col;
            }

            // move cursor up to the previous line row
            cursor.pos.y = (f)(G::yofs + prev_y * cursor.height);

            if (last_idx >= 0) {
                // place cursor on the last character and delete it
                cursor.pos.x = (f)(G::xofs + (uint)last_idx * cursor.width + cursor.width);
                //cursor.delete_char();
            } else {
                // previous line empty -> go to its start
                cursor.pos.x = (f)G::xofs;
            }
        }
    }


    
    // Render all characters at their *grid* positions
    for (uint i = 0; i < G::map.getHeight(); ++i)
    {
        for (uint j = 0; j < G::map.getWidth(); ++j) {
            char ch = G::map(j, i);

            if (ch != '\0')
            {
                Vector2 char_pos
                ={
                    (f)(G::xofs + j * cursor.width),
                    (f)(G::yofs + i * cursor.height)
                };

                gfx.write(ch, char_pos);
            }

            if (ch);
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
