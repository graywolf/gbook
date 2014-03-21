#include <vector>
#include <istream>
#include "user.h"

namespace gbook {
    namespace abook {
        /**
         * Saves users from "user" into "output" in format to which abook
         * understands.
         *
         * \param std::istream output Stream to which save users
         * \param std::vector<user> users Vector which users which shall be saved
         **/
        void save_to_file(std::istream output, const std::vector<user> & users);
    }
}
