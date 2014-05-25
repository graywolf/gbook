#ifndef GBOOK_GOOGLE_MANAGER_H
#define GBOOK_GOOGLE_MANAGER_H

#include <iostream>
#include <stdexcept>

#include "contacts.h"
#include "storage_manager.h"
#include "typedefs.h"
#include "user.h"
#include "user_reader.h"
#include "user_writer.h"

namespace gbook {
    /**
     * Class managing abook.
     **/
    class google_manager : public storage_manager {
    public:
        google_manager(contacts & c) : contacts_(c) {}
        virtual void add(user & u)          =0;
        virtual void update(user & u)       =0;
        virtual void remove(std::string id) =0;

        void load();
        virtual void fill_storage_changes(storage_changes & changes, user_list & last_sync);
    private:
        user_list current_;
        contacts & contacts_;

        int find_user_id_by_custom5(std::string);
        int find_user_id_by_custom5(user_list & haystack, std::string);
    };
}

#endif // GBOOK_GOOGLE_MANAGER_H
