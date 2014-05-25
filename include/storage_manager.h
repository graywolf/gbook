#ifndef GBOOK_STORAGE_MANAGER_H
#define GBOOK_STORAGE_MANAGER_H

#include "user.h"

namespace gbook {
    class storage_manager {
        virtual void add(user & u)          =0;
        virtual void update(user u)         =0;
        virtual void remove(std::string id) =0;
    };
}

#endif // GBOOK_STORAGE_MANAGER_H
