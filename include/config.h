#ifndef CONFIG_H
#define CONFIG_H

#include "version.h"

#include <map>
#include <string>

std::map<std::string, std::string> headers { {"GData-Version", "3.0"} };
std::string oauth_scope { "https://www.google.com/m8/feeds" };

std::string client_id { "YOUR-CLIENT-ID" };
std::string client_secret { "YOUR-CLIENT-SECRET" };

#endif // libgcon_config_H
