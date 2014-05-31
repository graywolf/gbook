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
        LOG_DEBUG3("Config file loaded and parsed.");
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
        LOG_DEBUG3("Config saved.");
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
        LOG_DEBUG3("general.abook_file is empty, using default: " << f)
    } else if(f.find('~') == 0) {
        string expanded_f = f;
        expanded_f.replace(0, 1, getenv("HOME"));
        LOG_DEBUG3("general.abook_file if home-relative, expanding " << f << " to " << expanded_f)
        f = expanded_f;
    }
    return f;
}