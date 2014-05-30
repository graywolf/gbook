#ifndef GBOOK_TYPEDEFS_H
#define GBOOK_TYPEDEFS_H

#include <vector>
#include <map>

#include "user.h"
#include "logger.h"

namespace gbook {
    typedef std::vector<user>           user_list;
    typedef std::vector<user *>         user_ptr_list;

    typedef jstation::severity severity;

    struct storage_changes;
}

#include "storage_manager.h"

namespace gbook {
    struct storage_changes {
        user_ptr_list new_users;
        user_ptr_list modified_users;
        user_list deleted_users;

        storage_manager * manager;
    };
}

#endif // GBOOK_TYPEDEFS_H