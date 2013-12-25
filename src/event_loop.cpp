#include "../include/event_loop.h"

#include <unistd.h>
#include <iostream>

using namespace std;

gbook::event_loop::event_loop(gbook::screen * screen) : m_screen({screen})
{
    //should be more than enough
    m_pollfds.reserve(8);
    
    //create pollfd for STDIN
    pollfd in = {0};
    in.fd = STDIN_FILENO;
    in.events = POLLIN;
    m_pollfds.push_back(in);
}

gbook::event_loop::~event_loop()
{

}

void gbook::event_loop::stop()
{
    m_stop = true;
}

void gbook::event_loop::start()
{
    while (!m_stop) {
        //poll for changes, timeout is set to 1s to be able to exit event_loop
        //if m_stop is set by some thread of something in resonable time (+- 1s)
        int poll_count = poll(m_pollfds.data(), m_pollfds.size(), 1000);
        if (poll_count < 0) {
            //TODO: somehow handle error here
        }
    }
}
