#ifndef GBOOK_MERGER_H
#define GBOOK_MERGER_H

#include <vector>
#include <map>

#include "typedefs.h"
#include "user.h"
#include "abook_manager.h"
#include "google_manager.h"

namespace gbook {
    class merger {
    public:
        merger(std::vector<std::string> & synced_c5s)
            : synced_c5s_(synced_c5s) {}

        abook_manager * abook_m;
        google_manager * google_m;

        /**
         * Set's state after last sync.
         *
         * \param user_list last_state state after last sync
         **/
        void set_last_state(user_list last_state) {
            last_state_ = last_state;
        }

        /**
         * Merges managers.
         **/
         void merge();

    private:
        void generate_storage_changes();
        void do_merge();
        void set_next_id_to_use();
        void update_abook();
        void update_google();

        inline bool user_synced(user & u) {
            return user_synced(u.custom5);
        }
        inline bool user_synced(user * u) {
            return user_synced(u->custom5);
        }
        inline bool user_synced(std::string c5) {
            // cannot use std::find cause of bug in gcc/stdlibc++/something
            for (std::string c5_in_v : synced_c5s_) {
                if (c5_in_v == c5) {
                    return true;
                }
            }
            return false;
        }

        user_list last_state_;
        storage_changes abook_changes_, google_changes_;
        int id_;
        std::vector<std::string> & synced_c5s_;
    };
}

#endif // GBOOK_MERGER_H
