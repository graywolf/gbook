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
        virtual void add(user & u)          =0;
        virtual void update(user & u)         =0;
        virtual void remove(std::string id) =0;

        void flush(std::ostream) const;
        void load(std::istream &);
        virtual void fill_storage_changes(storage_changes & changes, const user_list & last_sync);
    private:
        user_list current_;

        int find_user_id_by_custom5(std::string);
    };
}

#endif // GBOOK_ABOOK_MANAGER_H
