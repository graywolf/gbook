#include <stdexcept>

#include "config.h"

using namespace gbook;
using namespace std;

config::config(string config_file) {
    if (config_file.empty()) {
        config_file = getenv("HOME");
        config_file.append("/.config/gbook/config");
    }
    config_file_ = config_file;

    try {
        LOG_DEBUG("Reading config file " << config_file_);
        cfg_.readFile(config_file_);
        LOG_DEBUG2("Config file loaded and parsed.");
    } catch (IOException & ioe) {
        LOG_ERROR("Cannot read config file: " << ioe.what());
        throw ioe;
    } catch (ParseException & pe) {
        LOG_ERROR("Cannot parse config file: " << pe.what());
        throw pe;
    }
}

config::~config() {
    try {
        LOG_DEBUG("Saving config.");
        cfg_.writeFile(config_file_.c_str());
        LOG_DEBUG2("Config saved.");
    } catch (IOException & ioe) {
        LOG_ERROR("Cannot read config file: " << ioe.what());
        throw ioe;
    } catch (ParseException & pe) {
        LOG_ERROR("Cannot parse config file: " << pe.what());
        throw pe;
    }
}

config & config::get(string config_file) {
    static config instance(config_file);
    return instance;
}

string config::abook_file() {
    string f = get().cfg_.get("abook_file", string(), "general");
    if (f.empty()) {
        string f = getenv("HOME");
        f.append("/.abook/addressbook");
        LOG_DEBUG2("general.abook_file is empty, using default: " << f)
    } else if(f.find('~') == 0) {
        string expanded_f = f;
        expanded_f.replace(0, 1, getenv("HOME"));
        LOG_DEBUG2("general.abook_file if home-relative, expanding " << f << " to " << expanded_f)
        f = expanded_f;
    }
    return f;
}

string config::client_id() {
    string cid = get().cfg_.get("client_id", string(), "oauth2");
    if (cid.empty()) {
        LOG_ERROR("oauth2.client_id is empty, this should not be");
        throw runtime_error("oauth2.client_id is empty, this should not be");
    }
    return cid;
}

string config::client_secret() {
    string csec = get().cfg_.get("client_secret", string(), "oauth2");
    if (csec.empty()) {
        LOG_ERROR("oauth2.client_secret is empty, this should not be");
        throw runtime_error("oauth2.client_secret is empty, this should not be");
    }
    return csec;
}

string config::data_dir() {
    string data_dir = get().cfg_.get("data_dir", string(), "general");
    if (data_dir.empty()) {
        LOG_ERROR("general.data_dir is empty");
        throw runtime_error("general.data_dir is empty");
    } else if(data_dir.find('~') == 0) {
        string expanded_data_dir = data_dir;
        expanded_data_dir.replace(0, 1, getenv("HOME"));
        LOG_DEBUG2("general.data_dir if home-relative, expanding " << data_dir << " to " << expanded_data_dir)
        data_dir = expanded_data_dir;
    }
    return data_dir;
}

string config::last_state_file() {
    string dir = data_dir();
    return dir.append("/last_state");
}

string config::log_file() {
    string log_file = get().cfg_.get("log_file", string(), "general");
    if(log_file.find('~') == 0) {
        string expanded_log_file = log_file;
        expanded_log_file.replace(0, 1, getenv("HOME"));
        LOG_DEBUG2("general.log_file if home-relative, expanding " << log_file << " to " << expanded_log_file)
        log_file = expanded_log_file;
    }
    return log_file;
}
