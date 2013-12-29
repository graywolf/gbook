#include "../include/screen.h"

#include <locale.h>

#include <exception>

using namespace std;

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

    focus_window(add_window_to_map(new window(stdscr)));
    
    refresh();
}

int gbook::screen::add_window()
{
    window * w = new window();
    return add_window_to_map(w);
}

void gbook::screen::focus_window(int win_id)
{
    if (m_windows.find(win_id) != m_windows.end()) {
        m_active_window = win_id;
    } else {
        throw invalid_argument("Window with this id does not exist.");
    }
}

gbook::window * gbook::screen::get_active_window()
{
    return m_windows[m_active_window].get();
}

int gbook::screen::add_window_to_map(gbook::window * win)
{
    m_windows[++m_last_window_id] = unique_ptr<gbook::window>(win);
    return m_last_window_id;
}

gbook::screen::~screen()
{
    endwin();
}
