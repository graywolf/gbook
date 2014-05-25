#ifndef GBOOK_TYPEDEFS_H
#define GBOOK_TYPEDEFS_H

#include <vector>
#include <map>

#include "user.h"
#include "storage_manager.h"

namespace gbook {
    typedef std::vector<user>           user_list;
    typedef std::map<std::string, user> user_map;

    struct storage_changes {
        user_list new_users;
        user_map modified_users;
        user_map deleted_users;

        storage_manager * manager;
    };
}

#endif // GBOOK_TYPEDEFS_H