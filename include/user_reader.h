#ifndef GBOOK_ABOOK_USER_READER_H
#define GBOOK_ABOOK_USER_READER_H

#include <string>
#include <vector>
#include <istream>

#include "user.h"

namespace gbook {
    /**
        * Loads users from file and returns vector with instances of user.
        *
        * \param std::istream input Stream from which data will be read
        * \return std::vector<user> vector with users
        **/
    std::vector<user> load_users_from_stream(std::istream & input);
    /**
        * Parses string input and returns vector with instances of user
        * generated from this string.
        *
        * \param std::string input String which will be parsed
        * \return std::vector<user> vector with users
        **/
    std::vector<user> parse_string(std::string input);
}

#endif // GBOOK_ABOOK_USER_READER_H
