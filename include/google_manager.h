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
        google_manager() : contacts_() {}
        virtual void add(user & u);
        virtual void update(user & u);
        virtual void remove(user & u);

        void load();
        virtual void fill_storage_changes(storage_changes & changes, user_list & last_sync);
        virtual int get_max_id();
        virtual std::string name();
    private:
        user_list all_;
        contacts contacts_;
    };
}

#endif // GBOOK_GOOGLE_MANAGER_H
