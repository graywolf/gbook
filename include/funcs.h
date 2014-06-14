#ifndef GBOOK_FUNCS_H
#define GBOOK_FUNCS_H

#include "typedefs.h"
#include "user.h"

#include <algorithm>

namespace gbook {

    bool is_user_in(user & u, user_list & l);

    bool is_user_in(user & u, user_ptr_list & l);

    bool is_user_in(user * u, user_ptr_list & l);

    user_list::iterator find_user_in(user & u, user_list & l);

    user_ptr_list::iterator find_user_in(user & u, user_ptr_list & l);

    user_ptr_list::iterator find_user_in(user * u, user_ptr_list & l);
}

#endif // GBOOK_FUNCS_H
