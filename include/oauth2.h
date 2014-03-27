#ifndef libgcon_OAUTH2_H
#define libgcon_OAUTH2_H

#include "config.h"
#include "curl.h"
#include "jsoncpp/json/json.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

namespace gbook {

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
        oauth2(std::string conf_file) {
            conf_file_ = conf_file;
            std::ifstream inconf(conf_file);
            std::getline(inconf, access_token_);
            std::getline(inconf, refresh_token_);
        }
        ~oauth2() {
            std::ofstream outconf(conf_file_, std::ios::trunc);
            outconf << access_token_ << "\n" << refresh_token_;
        }
        user_data request_user_code() {
            try {
                curl c;
                c.url("https://accounts.google.com/o/oauth2/device/code");
                c.method(method::POST);
                c.form_field("client_id", client_id);
                c.form_field("scope", oauth_scope);
                c.execute();

                Json::Value root;
                Json::Reader reader;
                bool res = reader.parse(c.received_body(), root);
                if (!res) {
                    throw std::runtime_error(std::string("Cannot parse JSON: ").append(c.received_body()));
                }
                device_code_ = root.get("device_code", "").asString();
                user_code_ = root.get("user_code", "").asString();
                verification_url_ = root.get("verification_url", "").asString();
                expires_in_ = root.get("expires_in", 0).asInt();
                interval_ = root.get("interval", 0).asInt();

                request_ended_ = std::chrono::system_clock::now();

                return user_data(verification_url_, user_code_);
            } catch (std::runtime_error e) {
                throw std::runtime_error(std::string("Failed with error: ").append(e.what()));
            }
        }
        void begin_polling() {
            std::chrono::system_clock::time_point polling_ends = request_ended_ + std::chrono::seconds(expires_in_);
            while (polling_ends > std::chrono::system_clock::now()) {
                curl c;
                c.url("https://accounts.google.com/o/oauth2/token");
                c.method(method::POST);
                c.form_field("client_id", client_id);
                c.form_field("client_secret", client_secret);
                c.form_field("code", device_code_);
                c.form_field("grant_type", "http://oauth.net/grant_type/device/1.0");
                c.execute();

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
                    } else if (error == "slow_down") {
                        ++interval_;
                    } else {
                        throw std::runtime_error(std::string("Got uknown error: ").append(error));
                    }
                } else if (root.isMember("access_token")) {
                    access_token_ = root.get("access_token", "").asString();
                    token_type_ = root.get("token_type", "").asString();
                    refresh_token_ = root.get("refresh_token", "").asString();
                    access_expires_in_ = root.get("expires_in", 0).asInt();
                    return;
                }
                std::this_thread::sleep_for(std::chrono::seconds(interval_));
            }
            throw std::runtime_error("Polling timed out.");
        }
        std::string access_token() {
            return access_token_;
        }
        void refresh_access_token() {
            curl c;
            c.url("https://accounts.google.com/o/oauth2/token");
            c.method(method::POST);
            c.form_field("client_id", client_id);
            c.form_field("client_secret", client_secret);
            c.form_field("refresh_token", refresh_token_);
            c.form_field("grant_type", "refresh_token");
            c.execute();

            Json::Value root;
            Json::Reader reader;
            bool res = reader.parse(c.received_body(), root);
            if (!res) {
                throw std::runtime_error(std::string("Cannot parse JSON: ").append(c.received_body()));
            }
            access_token_ = root.get("access_token", "").asString();
            token_type_ = root.get("token_type", "").asString();
            access_expires_in_ = root.get("expires_in", 0).asInt();
        }
    private:
        std::chrono::system_clock::time_point request_ended_;

        std::string device_code_;
        std::string user_code_;
        std::string verification_url_;
        int expires_in_;
        int interval_;

        std::string access_token_, token_type_, refresh_token_;
        int access_expires_in_;

        std::string conf_file_;
    };
}

#endif // libgcon_OAUTH2_H
