#include "curl.h"

#include <memory.h>

size_t gbook::received_body_writer(char* ptr, size_t size, size_t nmemb, std::string received_body) {
    received_body.append(std::string(ptr, ptr + size * nmemb));
    return size * nmemb;
}
