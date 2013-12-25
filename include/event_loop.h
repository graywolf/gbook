#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include "../include/screen.h"
#include "../include/window.h"

#include <ncurses.h>
#include <vector>
#include <poll.h>
#include <memory>

namespace gbook {
    class event_loop {
    public:
        event_loop(gbook::screen* screen);
        ~event_loop();
        void start();
        void stop();
    private:
        gbook::screen * m_screen;
        std::vector<pollfd> m_pollfds;
        bool m_stop = {false};
    };
}

#endif //EVENT_LOOP_H
