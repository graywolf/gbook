#ifndef gbook_CURL_H
#define gbook_CURL_H

#include <stdexcept>
#include <map>
#include <string>
extern "C" {
    #include <curl/curl.h>
}

namespace gbook {
    enum class method {
        GET, POST, SIMPLE_POST, PUT, DELETE
    };
    size_t received_body_writer(char * ptr, size_t size, size_t nmemb, std::string received_body);
    /**
     * Overlay class for CURL.
     **/
    class curl {
    public:
        /**
         * Constructs and initializes this object.
         **/
        curl() : handle_(curl_easy_init()), first_form_item_(NULL), header_list_(NULL), method_(gbook::method::GET) {
            if (handle_ == NULL) {
                throw std::runtime_error("curl_easy_init return null");
            }
        }
        /**
         * Cleans up when we are done.
         **/
        ~curl() {
            if (first_form_item_ != NULL) {
                curl_formfree(first_form_item_);
            }
            if (header_list_ != NULL) {
                curl_slist_free_all(header_list_);
            }
            curl_easy_cleanup(handle_);
        }
        curl(curl &) =delete;
        curl(curl &&) =delete;
        curl & operator=(curl &) =delete;
        curl & operator=(curl && c) =delete;
        /**
         * Sets method to be used.
         *
         * \param gbook::method m Method to set as one to be used
         * \return gbook::curl& reference to (*this)
         **/
        curl & method(method m) {
            method_ = m;
            return * this;
        }
        /**
         * Sets form field and its value.
         *
         * \param std::string name form field name
         * \param std::string value form field value
         * \return gbook::curl& reference to (*this)
         **/
        curl & form_field(std::string name, std::string value) {
            auto ret = form_fields_.insert(std::pair<std::string, std::string>(name, value));
            if (!ret.second) {
                ret.first->second = value;
            }
            return * this;
        }
        /**
         * Sets header and its value.
         *
         * \param std::string name header name
         * \param std::string value header value
         * \return gbook::curl& reference to (*this)
         **/
        curl & header(std::string name, std::string value) {
            auto ret = headers_.insert(std::pair<std::string, std::string>(name, value));
            if (!ret.second) {
                ret.first->second = value;
            }
            return * this;
        }
        /**
         * Sets target url.
         *
         * \return gbook::curl& reference to (*this)
         **/
        curl & url(std::string url) {
            CURLcode res = curl_easy_setopt(handle_, CURLOPT_URL, url.c_str());
            if (res != CURLE_OK) {
                throw std::runtime_error(std::string("Cannot set url: ").append(curl_easy_strerror(res)));
            }
            return * this;
        }
        /**
         * Executes request.
         **/
        void execute() {
            CURLcode res;
            received_body_.erase();
            curl_easy_setopt(handle_, CURLOPT_WRITEFUNCTION, received_body_writer);
            curl_easy_setopt(handle_, CURLOPT_WRITEDATA, &received_body_);
            switch (method_) {
                case gbook::method::GET:
                    res = curl_easy_setopt(handle_, CURLOPT_HTTPGET, 1);
                    break;
                case gbook::method::POST:
                    process_form_fields();
                    res = curl_easy_setopt(handle_, CURLOPT_HTTPPOST, first_form_item_);
                    break;
                case gbook::method::SIMPLE_POST:
                    res = curl_easy_setopt(handle_, CURLOPT_POST, 1);
                    if (res != CURLE_OK) {
                        throw std::runtime_error(std::string("Cannot set method: ").append(curl_easy_strerror(res)));
                    }
                    res = curl_easy_setopt(handle_, CURLOPT_POSTFIELDS, post_body_.c_str());
                    if (res != CURLE_OK) {
                        throw std::runtime_error(std::string("Cannot set method: ").append(curl_easy_strerror(res)));
                    }
                    res = curl_easy_setopt(handle_, CURLOPT_POSTFIELDSIZE, post_body_.length());
                    break;
                case gbook::method::PUT:
                    res = curl_easy_setopt(handle_, CURLOPT_CUSTOMREQUEST, "PUT");
                    if (res != CURLE_OK) {
                        throw std::runtime_error(std::string("Cannot set method: ").append(curl_easy_strerror(res)));
                    }
                    res = curl_easy_setopt(handle_, CURLOPT_POSTFIELDS, post_body_.c_str());
                    if (res != CURLE_OK) {
                        throw std::runtime_error(std::string("Cannot set method: ").append(curl_easy_strerror(res)));
                    }
                    res = curl_easy_setopt(handle_, CURLOPT_POSTFIELDSIZE, post_body_.length());
                    break;
                case gbook::method::DELETE:
                    res = curl_easy_setopt(handle_, CURLOPT_CUSTOMREQUEST, "DELETE");
                    break;
            }
            if (res != CURLE_OK) {
                throw std::runtime_error(std::string("Cannot set method: ").append(curl_easy_strerror(res)));
            }
            process_headers();
            curl_easy_setopt(handle_, CURLOPT_HTTPHEADER, header_list_);
            res = curl_easy_perform(handle_);
            if (res != CURLE_OK) {
                throw std::runtime_error(std::string("Failed with error: ").append(curl_easy_strerror(res)));
            }
        }
        /**
         * Sets body for POST request.
         **/
        void set_body(std::string body) {
            post_body_ = body;
        }
        /**
         * Gets body returned from the request.
         *
         * \return std::string
         **/
        std::string received_body() const {
            return received_body_;
        }
        /**
         * Returns status code.
         **/
        long return_code() {
            long http_code;
            curl_easy_getinfo(handle_, CURLINFO_RESPONSE_CODE, &http_code);
            return http_code;
        }
    private:
        CURL * handle_;
        std::map<std::string, std::string> form_fields_;
        std::map<std::string, std::string> headers_;
        curl_httppost * first_form_item_;
        curl_slist * header_list_;
        gbook::method method_;
        std::string post_body_;
        std::string received_body_;

        curl & process_form_fields() {
            curl_httppost * lastptr = NULL;

            for (auto field : form_fields_) {
                curl_formadd(
                    &first_form_item_,
                    &lastptr,
                    CURLFORM_COPYNAME, field.first.c_str(),
                             CURLFORM_COPYCONTENTS, field.second.c_str(),
                             CURLFORM_END
                );
            }
            return * this;
        }
        curl & process_headers() {
            for (auto header : headers_) {
                std::string header_line = header.first;
                header_line.append(": ");
                header_line.append(header.second);
                header_list_ = curl_slist_append(
                    header_list_,
                    header_line.c_str()
                );
            }
            return * this;
        }
    };
}

#endif // gbook_CURL_H
