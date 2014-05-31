#ifndef CONFIG_H
#define CONFIG_H

#include "cfg_parser/cfg_parser.h"
#include "version.h"
#include "macros.h"

namespace gbook {
    class config {
    public:
        static config & get(std::string config_file = "");
        static std::string abook_file();
    private:
        config(std::string config_file);
        ~config();

        std::string config_file_;
        ConfigParser cfg_;
    };
}

#endif // CONFIG_H
