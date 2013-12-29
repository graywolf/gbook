#ifndef SCREEN_H
#define SCREEN_H

#include "window.h"

#include <ncurses.h>

#include <map>
#include <memory>

namespace gbook {
    class screen {
    public:
        enum class input_mode { none, raw, cbreak };
        enum class echo_mode { none, echo, noecho };
        screen(input_mode in_mode = input_mode::raw, echo_mode e_mode = echo_mode::noecho, bool want_keypad = true, bool set_locale = true);
        ~screen();

        int add_window();
        void focus_window(int win_id);
        window * get_active_window();
    private:
        std::map<int, std::unique_ptr<window>> m_windows;
        int m_active_window;
        int m_last_window_id = -1;
        int add_window_to_map(window * win);
    };
}

#endif //SCREEN_H
