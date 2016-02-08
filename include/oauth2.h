#ifndef libgcon_OAUTH2_H
#define libgcon_OAUTH2_H

#include "config.h"
#include "curl.h"
#include "json/json.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

namespace gbook {

    const std::string oauth_scope { "https://www.google.com/m8/feeds" };

    /**
     * Class providing limited oauth2 functionality,
     * for now only device codes.
     **/
    class oauth2 {
    public:
        /**
         * Requests Google for details about how to authenticate user.
         *
         * \throws std::runtime_exception when something was wrong
         **/
        void request_user_code(std::string &, std::string &);

        /**
         * Begins polling for access & refresh token. Ends 1) when received tokens 2) user code expires.
         *
         * \throws std::runtime_exception when something was wrong
         */
        void begin_polling();

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
         *
         * \throws std::runtime_exception when something was wrong
         **/
        void refresh_access_token();
    private:
        std::chrono::system_clock::time_point request_ended_;

        std::string device_code_;
        int expires_in_;
        int interval_;

        std::string access_token_,
                    token_type_,
                    refresh_token_;
        int access_expires_in_;
    };
}

#endif // libgcon_OAUTH2_H
