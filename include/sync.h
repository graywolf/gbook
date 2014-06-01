#ifndef SYNC_H
#define SYNC_H

#include <string>
#include <map>
#include <set>
#include <iostream>

#include "user.h"
#include "oauth2.h"
#include "merger.h"

namespace gbook {
    /**
     * Class handling syncing between abook and google contacts.
     **/
    class sync {
    public:
        /**
         * Do the whole sync.
         **/
        void do_sync();
    private:
        void ensure_token();

        void load_managers();
        void load_last_state();
        void load_abook();
        void load_google();

        oauth2 o2_;
        merger m_;
    };
}

#endif // SYNC_H
