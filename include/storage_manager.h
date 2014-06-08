#ifndef GBOOK_STORAGE_MANAGER_H
#define GBOOK_STORAGE_MANAGER_H

namespace gbook {
    class storage_manager;
}

#include "typedefs.h"
#include "user.h"

namespace gbook {
    /**
     * Base class for contact managers.
     **/
    class storage_manager {
    public:
        /**
         * Adds new user into system managed by this manager. Can throw error
         * if user with id already exists, but manager has no obligation to
         * check for it.
         *
         * \param user & u New user, custom5 must be already set!
         * \throw std::invalid_argument When user with custom5 already exists. Function has no obligation to check for this.
         **/
        virtual void add(user & u) =0;
        /**
         * Updates user.
         *
         * \param user & u
         * \throw std::invalid_argument If id for this manager is not set on user. If user with id doesn't exist (no obligation to check).
         **/
        virtual void update(user & u) =0;
        /**
         * Removes user with this id.
         *
         * \param std::string id Id of user to be removed.
         **/
        virtual void remove(std::string id) =0;

        /**
         * Fills storage_changes with changes base on last_sync.
         *
         * \param storage_changes & changes Structure to fill
         * \param const user_list & last_sync State after last sync
         **/
        virtual void fill_storage_changes(storage_changes & changes, user_list & last_sync) =0;
    };
}

#endif // GBOOK_STORAGE_MANAGER_H
