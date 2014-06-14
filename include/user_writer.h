#ifndef GBOOK_ABOOK_USER_WRITER_H
#define GBOOK_ABOOK_USER_WRITER_H

#include <vector>
#include <iostream>

#include "typedefs.h"
#include "user.h"

namespace gbook {
    /**
        * Removes new lines from string. Is group_mode, also replaces ',' with '_'.
        **/
    std::string sanitize(std::string input, bool group_mode = false);
}


std::ostream& operator<<(std::ostream& os, const gbook::user& u);

std::ostream& operator<<(std::ostream& os, const gbook::user_list& users);

std::ostream& operator<<(std::ostream& os, const gbook::user_ptr_list& users);

#endif // GBOOK_ABOOK_USER_WRITER_H
