#include "oauth2.h"
#include "config.h"

using namespace gbook;
using namespace std;

void oauth2::request_user_code(
    std::string & verification_url,
    std::string & user_code
) {
    LOG_DEBUG2("Requesting device code from google")
    curl c;
    c.url("https://accounts.google.com/o/oauth2/device/code");
    c.method(method::POST);
    c.form_field("client_id", config::client_id());
    c.form_field("scope", oauth_scope);
    c.execute();
    LOG_DEBUG3("Got response.\n" << c.received_body())

    Json::Value root;
    Json::Reader reader;
    bool res = reader.parse(c.received_body(), root);
    if (!res) {
        LOG_ERROR(
            "Something went wrong while parsing:\n" << c.received_body()
        )
        throw std::runtime_error(
            std::string("Cannot parse JSON: ").append(c.received_body())
        );
    }
    if (root.isMember("error")) {
        LOG_ERROR("Following body contains error:\n" << c.received_body())
        throw std::runtime_error(
            std::string("Received error while requesting tokens, error: ")
                .append(root.get("error", "").asString())
                .append(", description: ")
                .append(root.get("error_description", "").asString())
            );
    }
    LOG_DEBUG3("No error in the response, processing it")

    // things to return to the user
    user_code = root.get("user_code", "").asString();
    verification_url = root.get("verification_url", "").asString();

    // and internal stuff
    device_code_ = root.get("device_code", "").asString();
    expires_in_ = root.get("expires_in", 0).asInt();
    interval_ = root.get("interval", 0).asInt();
    request_ended_ = std::chrono::system_clock::now();

    LOG_DEBUG2("Device code read")
}

void oauth2::begin_polling() {
    LOG_DEBUG2("Polling for tokens")
    std::chrono::system_clock::time_point polling_ends
        = request_ended_ + std::chrono::seconds(expires_in_);
    while (polling_ends > std::chrono::system_clock::now()) {
        LOG_DEBUG3("Looping...")
        curl c;
        c.url("https://accounts.google.com/o/oauth2/token");
        c.method(method::POST);
        c.form_field("client_id", config::client_id());
        c.form_field("client_secret", config::client_secret());
        c.form_field("code", device_code_);
        c.form_field("grant_type", "http://oauth.net/grant_type/device/1.0");
        c.execute();
        LOG_DEBUG3("Response received.\n" << c.received_body())

        Json::Value root;
        Json::Reader reader;
        bool res = reader.parse(c.received_body(), root);
        if (!res) {
            LOG_ERROR(
                "Something went wrong while parsing:\n" << c.received_body()
            )
            throw std::runtime_error(
                std::string("Cannot parse JSON: ").append(c.received_body())
            );
        }
        if (root.isMember("error")) {
            std::string error = root.get("error", "").asString();
            if (error == "authorization_pending") {
                //this is normal
                LOG_DEBUG3("No tokens yet, waiting some more")
            } else if (error == "slow_down") {
                LOG_DEBUG3("We are asking to fast, slowing down a little bit.")
                ++interval_;
            } else {
                LOG_ERROR(
                    "Following body contains an error:\n" << c.received_body()
                )
                throw std::runtime_error(
                    std::string("Got uknown error: ").append(error)
                );
            }
        } else if (root.isMember("access_token")) {
            config::access_token(root.get("access_token", "").asString());
            token_type_ = root.get("token_type", "").asString();
            config::refresh_token(root.get("refresh_token", "").asString());
            access_expires_in_ = root.get("expires_in", 0).asInt();

            LOG_DEBUG2("We've got tokens, breaking loop.")
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(interval_));
    }
    LOG_ERROR("Polling timeouted. Throwing exception.")
    throw std::runtime_error("Polling timed out.");
}

void oauth2::refresh_access_token() {
    LOG_DEBUG2("Requesting token refresh")
    curl c;
    c.url("https://accounts.google.com/o/oauth2/token");
    c.method(method::POST);
    c.form_field("client_id", config::client_id());
    c.form_field("client_secret", config::client_secret());
    c.form_field("refresh_token", config::refresh_token());
    c.form_field("grant_type", "refresh_token");
    c.execute();
    LOG_DEBUG3("Got response.\n" << c.received_body())

    Json::Value root;
    Json::Reader reader;
    bool res = reader.parse(c.received_body(), root);
    if (!res) {
        LOG_ERROR(
            "Something went wrong while parsing:\n" << c.received_body()
        )
        throw std::runtime_error(std::string("Cannot parse JSON: ").append(c.received_body()));
    }
    if (root.isMember("error")) {
        LOG_ERROR(
            "Following body contains an error:\n" << c.received_body()
        )
        throw std::runtime_error(
            std::string("Received error while refreshing tokens, error: ")
                .append(root.get("error", "").asString())
                .append(", description: ")
                .append(root.get("error_description", "").asString())
            );
    }

    config::access_token(root.get("access_token", "").asString());
    token_type_ = root.get("token_type", "").asString();
    access_expires_in_ = root.get("expires_in", 0).asInt();
    LOG_DEBUG2("Tokens refreshed.")
}