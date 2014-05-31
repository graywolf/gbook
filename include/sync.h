#ifndef SYNC_H
#define SYNC_H

#include <string>
#include <map>
#include <set>
#include <iostream>

#include "user.h"
#include "oauth2.h"

namespace gbook {
    /**
     * Class handling syncing between abook and google contacts.
     **/
    class sync {
    public:
        /**
         * Constructs sync instance and assure we have access token. Either authenticates user
         * or refreshes access token.
         **/
        sync(std::string abook_folder);
        /**
         * Do the whole sync.
         **/
        void do_sync();
    private:
        oauth2 o2_;
    };
}

#endif // SYNC_H
