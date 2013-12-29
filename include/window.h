#ifndef WINDOW_H
#define WINDOW_H

#include <ncurses.h>

namespace gbook {
    class window {
    public:
        window();
        window(WINDOW * win_ptr) : m_window(win_ptr) {};
        ~window();
        
        WINDOW * get_window_ptr();
    private:
        WINDOW * m_window;
    };
}

#endif //WINDOW_H
