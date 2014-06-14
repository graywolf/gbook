#ifndef CONFIG_H
#define CONFIG_H

#include "cfg_parser/cfg_parser.h"
#include "version.h"
#include "macros.h"

namespace gbook {
    class config {
    public:
        /**
         * Get's instance of config.
         *
         * \return gbook::config &
         **/
        static config & get(std::string config_file = "");

        /**
         * Get's abook's address file path.
         *
         * Three cases:
         *  1) not specify in config
         *      -> use ~/.abook/addressbook
         *
         *  2) path specified starts with '~'
         *      -> expand '~' to home dir and return result
         *
         *  3) other
         *      -> return specified value
         *
         * \return std::string
         **/
        static std::string abook_file();

        /**
         * Get's client_id.
         *
         * \throw std::runtime_error if not specified in config file
         * \return std::string
         **/
        static std::string client_id();

        /**
         * Get's client_secret.
         *
         * \throw std::runtime_error if not specified in config file
         * \return std::string
         **/
        static std::string client_secret();

        /**
         * Gets access token from config file.
         *
         * \return std::string
         **/
        static std::string access_token() {
            return get().cfg_.get("access_token", std::string(), "oauth2");
        }

        /**
         * Sets access token.
         **/
        static void access_token(std::string new_token) {
            get().cfg_.set("access_token", new_token, "oauth2");
        }

        /**
         * Gets refresh token from config file.
         *
         * \return std::string
         **/
        static std::string refresh_token() {
            return get().cfg_.get("refresh_token", std::string(), "oauth2");
        }

        /**
         * Sets refresh token.
         **/
        static void refresh_token(std::string new_token) {
            get().cfg_.set("refresh_token", new_token, "oauth2");
        }

        /**
         * Data dir.
         *
         * \return std::string
         **/
        static std::string data_dir();

        /**
         * Last state file.
         *
         * \return std::string
         **/
        static std::string last_state_file();

        /**
         * Path to log file.
         *
         * \return std::string
         **/
        static std::string log_file();
    private:
        config(std::string config_file);
        ~config();

        std::string config_file_;
        ConfigParser cfg_;
    };
}

#endif // CONFIG_H
