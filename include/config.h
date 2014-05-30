#ifndef CONFIG_H
#define CONFIG_H

#include "cfg_parser/cfg_parser.h"
#include "version.h"
#include "macros.h"

namespace gbook {
    class config {
    public:
        static config & get(std::string path = "") {
            static config instance(path);
            return instance;
        }
    private:
        config(std::string config_file) {
            std::string config_file;
            if (path.empty()) {
                config_file = getenv("HOME");
                config_file.append("/.config/gbook/config");
            } else {
                config_file = path;
            }
            config_file_ = config_file;

            try {
                LOG_DEBUG("Reading config file " << config_file_);
                cfg_.readFile(config_file_);
                LOG_DEBUG3("Config file loaded and parsed.");
            } catch (libconfig::FileIOException fioe) {
                throw std::invalid_argument(
                    std::string("Config file '")
                        .append(config_file_)
                        .append("' cannot be open.")
                );
            } catch (libconfig::ParseException pe) {
                throw std::invalid_argument(
                    std::string("Config file cannot be parsed, error ")
                        .append(std::to_string(pe.getLine()))
                        .append(":")
                        .append(pe.getError())
                );
            }
        }
        ~config() {
            try {
                LOG_DEBUG("Saving config.");
                cfg_.writeFile(config_file_.c_str());
                LOG_DEBUG3("Config saved.");
            } catch (libconfig::FileIOException fioe) {
                throw std::invalid_argument(
                    std::string("Config file '")
                        .append(config_file_)
                        .append("' cannot be open.")
                );
            } catch (libconfig::ParseException pe) {
                throw std::invalid_argument(
                    std::string("Config file cannot be parsed, error ")
                        .append(std::to_string(pe.getLine()))
                        .append(":")
                        .append(pe.getError())
                );
            }
        }
        std::string config_file_;
        ConfigParser cfg_;
    };
}

#endif // CONFIG_H
