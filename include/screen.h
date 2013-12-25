#ifndef SCREEN_H
#define SCREEN_H

#include "window.h"

#include <ncurses.h>

namespace gbook {
    class screen {
    public:
        enum class input_mode { none, raw, cbreak };
        enum class echo_mode { none, echo, noecho };
        screen(input_mode in_mode = input_mode::raw, echo_mode e_mode = echo_mode::noecho, bool want_keypad = true, bool set_locale = true);
        ~screen();
    private:
    };
}

#endif //SCREEN_H
