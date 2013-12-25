#include "../include/screen.h"

#include <locale.h>

gbook::screen::screen(gbook::screen::input_mode in_mode, gbook::screen::echo_mode e_mode, bool want_keypad, bool set_locale)
{
    if (set_locale) {
        setlocale(LC_CTYPE, "");
    }
    
    initscr();
    
    switch (in_mode) {
        case gbook::screen::input_mode::cbreak:
            cbreak();
            break;
        case gbook::screen::input_mode::raw:
            raw();
            break;
        case gbook::screen::input_mode::none:
        default:
            break;
    }
    
    switch (e_mode) {
        case gbook::screen::echo_mode::echo:
            echo();
            break;
        case gbook::screen::echo_mode::noecho:
            noecho();
            break;
        default:
            break;
    }
    
    if (want_keypad) {
        keypad(stdscr, true);
    }
    
    refresh();
}

gbook::screen::~screen()
{
    endwin();
}
