#ifndef CONFIG_H
#define CONFIG_H

#include "cfg_parser/cfg_parser.h"
#include "version.h"
#include "macros.h"

namespace gbook {
    class config {
    public:
        static ConfigParser & get(std::string config_file = "") {
            static config instance(config_file);
            return instance.cfg_;
        }
    private:
        config(std::string config_file) {
            if (config_file.empty()) {
                config_file = getenv("HOME");
                config_file.append("/.config/gbook/config");
            }
            config_file_ = config_file;

            try {
                LOG_DEBUG("Reading config file " << config_file_);
                cfg_.readFile(config_file_);
                LOG_DEBUG3("Config file loaded and parsed.");
            } catch (IOException & ioe) {
                LOG_ERROR("Cannot read config file: " << ioe.what());
                throw ioe;
            } catch (ParseException & pe) {
                LOG_ERROR("Cannot parse config file: " << pe.what());
                throw pe;
            }
        }
        ~config() {
            try {
                LOG_DEBUG("Saving config.");
                cfg_.writeFile(config_file_.c_str());
                LOG_DEBUG3("Config saved.");
            } catch (IOException & ioe) {
                LOG_ERROR("Cannot read config file: " << ioe.what());
                throw ioe;
            } catch (ParseException & pe) {
                LOG_ERROR("Cannot parse config file: " << pe.what());
                throw pe;
            }
        }
        std::string config_file_;
        ConfigParser cfg_;
    };
}

#endif // CONFIG_H
