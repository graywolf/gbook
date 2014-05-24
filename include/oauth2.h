#ifndef libgcon_OAUTH2_H
#define libgcon_OAUTH2_H

#include "config.h"
#include "curl.h"
#include "jsoncpp/json/json.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <libconfig.h++>

namespace gbook {

    const std::string oauth_scope { "https://www.google.com/m8/feeds" };

    /**
     * Structure containing result of authentication request. Verification url and user code to input.
     **/
    struct user_data {
        std::string verification_url;
        std::string user_code;
        user_data(std::string verification_url_, std::string user_code_)
            : verification_url(verification_url_), user_code(user_code_) {}
    };

    /**
     * Class providing limited oauth2 functionality,
     * for now only device codes.
     **/
    class oauth2 {
    public:
        oauth2(oauth2 & other) =delete;
        /**
         * Constructor reads auth & refresh token from data file.
         **/
        oauth2(std::string conf_file) {
            conf_file_ = conf_file;

            try {
                libconfig::Config c;
                std::cout << "Reading o2 config file " << conf_file_ << std::endl;
                c.readFile(conf_file_.c_str());
                if (!(
                    c.lookupValue("client_id", client_id_)
                    &&
                    c.lookupValue("client_secret", client_secret_)
                )) {
                    throw std::invalid_argument(
                        std::string("O2 config file doesn't contain client_id and client_secret.")
                    );
                }

                c.lookupValue("access_token", access_token_);
                c.lookupValue("refresh_token", refresh_token_);

                std::cout << "o2 config loaded." << std::endl;
            } catch (libconfig::FileIOException fioe) {
                throw std::invalid_argument(
                    std::string("O2 config file '")
                        .append(conf_file_)
                        .append("' cannot be open.")
                );
            } catch (libconfig::ParseException pe) {
                throw std::invalid_argument(
                    std::string("O2 config file cannot be parsed, error ")
                        .append(std::to_string(pe.getLine()))
                        .append(":")
                        .append(pe.getError())
                );
            }
        }
        /**
         * Destructor writes auth & refresh token to data file.
         **/
        ~oauth2() {
            try {
                std::cout << "Saving access & refresh tokens..." << std::endl;
                libconfig::Config c;
                c.readFile(conf_file_.c_str());
                libconfig::Setting & root = c.getRoot();

                if (!root.exists("access_token")) {
                    root.add("access_token", libconfig::Setting::TypeString) = access_token_;
                } else {
                    c.lookup("access_token") = access_token_;
                }

                if (!root.exists("refresh_token")) {
                    root.add("refresh_token", libconfig::Setting::TypeString) = refresh_token_;
                } else {
                    c.lookup("refresh_token") = refresh_token_;
                }

                c.writeFile(conf_file_.c_str());
                std::cout << "Tokens saved." << std::endl;
            } catch (libconfig::FileIOException fioe) {
                throw std::invalid_argument(
                    std::string("O2 config file '")
                        .append(conf_file_)
                        .append("' cannot be open.")
                );
            } catch (libconfig::ParseException pe) {
                throw std::invalid_argument(
                    std::string("O2 config file cannot be parsed, error ")
                        .append(std::to_string(pe.getLine()))
                        .append(":")
                        .append(pe.getError())
                );
            }
        }
        /**
         * Requests Google for details about how to authenticate user.
         *
         * \return user_data
         **/
        user_data request_user_code() {
            std::cout << "Requesting device code from google... ";
            curl c;
            c.url("https://accounts.google.com/o/oauth2/device/code");
            c.method(method::POST);
            c.form_field("client_id", client_id_);
            c.form_field("scope", oauth_scope);
            c.execute();
            std::cout << "Got response.\n" << c.received_body() << std::endl;

            Json::Value root;
            Json::Reader reader;
            bool res = reader.parse(c.received_body(), root);
            if (!res) {
                throw std::runtime_error(std::string("Cannot parse JSON: ").append(c.received_body()));
            }
            if (root.isMember("error")) {
                throw std::runtime_error(
                    std::string("Received error while requesting tokens, error: ")
                        .append(root.get("error", "").asString())
                        .append(", description: ")
                        .append(root.get("error_description", "").asString())
                    );
            }
            std::cout << "No errors, reading data." << std::endl;
            std::cout << c.received_body() << std::endl;
            device_code_ = root.get("device_code", "").asString();
            user_code_ = root.get("user_code", "").asString();
            verification_url_ = root.get("verification_url", "").asString();
            expires_in_ = root.get("expires_in", 0).asInt();
            interval_ = root.get("interval", 0).asInt();

            request_ended_ = std::chrono::system_clock::now();

            std::cout << "Device code read." << std::endl;

            return user_data(verification_url_, user_code_);
        }
        /**
         * Begins polling for access & refresh token. Ends 1) when received tokens 2) user code expires.
         */
        void begin_polling() {
            std::cout << "Polling for tokens..." << std::endl;
            std::chrono::system_clock::time_point polling_ends = request_ended_ + std::chrono::seconds(expires_in_);
            while (polling_ends > std::chrono::system_clock::now()) {
                std::cout << "Loop... ";
                curl c;
                c.url("https://accounts.google.com/o/oauth2/token");
                c.method(method::POST);
                c.form_field("client_id", client_id_);
                c.form_field("client_secret", client_secret_);
                c.form_field("code", device_code_);
                c.form_field("grant_type", "http://oauth.net/grant_type/device/1.0");
                c.execute();
                std::cout << "Response received.\n" << c.received_body() << std::endl;

                Json::Value root;
                Json::Reader reader;
                bool res = reader.parse(c.received_body(), root);
                if (!res) {
                    throw std::runtime_error(std::string("Cannot parse JSON: ").append(c.received_body()));
                }
                if (root.isMember("error")) {
                    std::string error = root.get("error", "").asString();
                    if (error == "authorization_pending") {
                        //this is normal
                        std::cout << "No tokens yet, waiting." << std::endl;
                    } else if (error == "slow_down") {
                        std::cout << "We are asking to fast, slowing down a little bit." << std::endl;
                        ++interval_;
                    } else {
                        throw std::runtime_error(std::string("Got uknown error: ").append(error));
                    }
                } else if (root.isMember("access_token")) {
                    access_token_ = root.get("access_token", "").asString();
                    token_type_ = root.get("token_type", "").asString();
                    refresh_token_ = root.get("refresh_token", "").asString();
                    access_expires_in_ = root.get("expires_in", 0).asInt();

                    std::cout << "We've got tokens, breaking loop." << std::endl;
                    return;
                }
                std::this_thread::sleep_for(std::chrono::seconds(interval_));
            }
            throw std::runtime_error("Polling timed out.");
        }
        /**
         * Getter method for access token.
         *
         * \return std::string access token
         **/
        std::string access_token() {
            return access_token_;
        }
        /**
         * Refresh access token.
         **/
        void refresh_access_token() {
            std::cout << "Requesting token refresh... ";
            curl c;
            c.url("https://accounts.google.com/o/oauth2/token");
            c.method(method::POST);
            c.form_field("client_id", client_id_);
            c.form_field("client_secret", client_secret_);
            c.form_field("refresh_token", refresh_token_);
            c.form_field("grant_type", "refresh_token");
            c.execute();
            std::cout << "Got response.\n" << c.received_body() << std::endl;

            Json::Value root;
            Json::Reader reader;
            bool res = reader.parse(c.received_body(), root);
            if (!res) {
                throw std::runtime_error(std::string("Cannot parse JSON: ").append(c.received_body()));
            }
            if (root.isMember("error")) {
                throw std::runtime_error(
                    std::string("Received error while refreshing tokens, error: ")
                        .append(root.get("error", "").asString())
                        .append(", description: ")
                        .append(root.get("error_description", "").asString())
                    );
            }
            access_token_ = root.get("access_token", "").asString();
            token_type_ = root.get("token_type", "").asString();
            access_expires_in_ = root.get("expires_in", 0).asInt();
            std::cout << "Tokens refreshed." << std::endl;
        }
    private:
        std::chrono::system_clock::time_point request_ended_;

        std::string device_code_;
        std::string user_code_;
        std::string verification_url_;
        int expires_in_;
        int interval_;

        std::string access_token_, token_type_, refresh_token_, client_id_, client_secret_;
        int access_expires_in_;

        std::string conf_file_;
    };
}

#endif // libgcon_OAUTH2_H
