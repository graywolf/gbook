#ifndef GBOOK_ABOOK_MANAGER_H
#define GBOOK_ABOOK_MANAGER_H

#include <iostream>
#include <stdexcept>

#include "storage_manager.h"
#include "typedefs.h"
#include "user.h"
#include "user_reader.h"
#include "user_writer.h"

namespace gbook {
    /**
     * Class managing abook.
     **/
    class abook_manager : public storage_manager {
    public:
        virtual void add(user & u);
        virtual void update(user & u);
        virtual void remove(user & u);

        void flush(std::ostream &) const;
        void load(std::istream &);
        virtual void fill_storage_changes(storage_changes & changes, user_list & last_sync);
        virtual int get_max_id();
        virtual std::string name();
    private:
        user_ptr_list current_;
        user_list all_;
    };
}

#endif // GBOOK_ABOOK_MANAGER_H
