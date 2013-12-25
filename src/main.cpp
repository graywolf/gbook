#include <iostream>

#include "../include/screen.h"
#include "../include/window.h"
#include "../include/event_loop.h"

#include "../include/config.h"

using namespace gbook;

int main(int argc, char **argv) {
    gbook::screen s;
    window w;
    event_loop el(&s);
    el.start();
    return 0;
}
