#ifndef GBOOK_ABOOK_USER_READER_H
#define GBOOK_ABOOK_USER_READER_H

#include <string>
#include <vector>
#include <istream>

#include "user.h"
#include "typedefs.h"

namespace gbook {
    /**
        * Loads users from file and returns vector with instances of user.
        *
        * \param std::istream input Stream from which data will be read
        * \return user_list vector with users
        **/
    user_list load_users_from_stream(std::istream & input);
    /**
        * Parses string input and returns vector with instances of user
        * generated from this string.
        *
        * \param std::string input String which will be parsed
        * \return user_list vector with users
        **/
    user_list parse_string(std::string input);
}

#endif // GBOOK_ABOOK_USER_READER_H
