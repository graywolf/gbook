#ifndef GBOOK_MERGER_H
#define GBOOK_MERGER_H

#include <vector>
#include <map>

#include "typedefs.h"
#include "user.h"
#include "storage_manager.h"

namespace gbook {
    class merger {
    private:
        struct manager_record {
            storage_manager * manager_;
            bool owner_;
            storage_changes changes_;
        };
    public:
        ~merger();
        /**
         * Sets primary manager
         *
         * \param storage_manager * manager storage manager
         * \param bool owner is merger owner of the manager?
         **/
        void set_primary(storage_manager * manager, bool owner = true) {
            primary_.manager_ = manager;
            primary_.owner_ = owner;
        }

        /**
         * Adds secondary manager
         *
         * \param storage_manager * manager storage manager
         * \param bool owner is merger owner of the manager?
         **/
        void add_secondary(storage_manager * manager, bool owner = true) {
            manager_record mr;
            mr.manager_ = manager;
            mr.owner_ = owner;
            secondaries_.push_back(mr);
        }

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
        void deduplicate_changes();
        void do_merge();
        int get_next_id_to_use();
        void update_secondary(manager_record & secondary, int &);
        void update_primary(int &);
        void fill_secondary_changes();

        user_list last_state_;
        manager_record primary_;
        std::vector<manager_record> secondaries_;
        storage_changes secondary_changes_;
    };
}

#endif // GBOOK_MERGER_H
