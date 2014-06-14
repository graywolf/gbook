#ifndef GBOOK_TYPEDEFS_H
#define GBOOK_TYPEDEFS_H

#include <vector>
#include <list>
#include <map>

#include "user.h"
#include "logger.h"

namespace gbook {
    typedef std::list<user>   user_list;
    typedef std::list<user *> user_ptr_list;

    typedef jstation::severity severity;

    struct storage_changes {
        user_ptr_list new_users;
        user_ptr_list modified_users;
        user_ptr_list deleted_users;
    };
}

#endif // GBOOK_TYPEDEFS_H
