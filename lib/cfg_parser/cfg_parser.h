//
//  cfg_parser.h
//  Config File Parser
//
//  Created by Clemens Wacha
//  Version 1.0
//  Copyright (c) 2006 Clemens Wacha. All rights reserved
//

#ifndef _CFG_PARSER_H
#define _CFG_PARSER_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define HAVE_SSTREAM

#include <list>
#include <string>
#include <vector>

#ifdef HAVE_SSTREAM
#include <sstream>
#endif

#define CFG_MAXLENGTH 128



class ConfigParser {
public:
    ConfigParser(const std::string& in_file = "");
    ~ConfigParser();

    // loads the given configuration file
    int readFile(const std::string& in_file);
    // writes the current configuration to the given file. I file not given, write back into the inputfile
    int writeFile(const std::string& out_file = "");
    // returns filname of loaded file
    void getFilename(std::string& in_file);
    // empty lines in config file are written back to the out_file if enable = true.
    // this function has to be called before readFile() since it defaults to false
    void preserveEmptyLines(bool enable = true);


    // return variable from configuration (if non-existent, value remains unchanged)
    void getVar(const std::string& keyword, std::string& value, const std::string& section = "");
    // sets variable in configuration (section and/or variable are added if non-existent)
    // if keyword is "-" then comment is appended to the given section. It is not possible to change the comment afterwards
    void setVar(const std::string& keyword, const std::string& value, const std::string& section = "", const std::string& comment = "");
    // deletes variable from configuration
    void delVar(const std::string& keyword, const std::string& section = "");

#ifdef HAVE_SSTREAM
    // returns value converted to type T if found, default_value else
    // Remark: does NOT work with char*. Use std::string or getCString() instead
    template <typename T> T get(const std::string& keyword, const T& default_value, const std::string& section = "") {
        std::list<ConfigEntry>::iterator it;

        if( findEntry(keyword, section, it) ) {
            std::istringstream i(it->value);
            T x;
            if(!(i >> x)) return default_value;
            return x;
        }
        return default_value;
    }

    // set value of keyword (internally converted to string). (works with char* too)
    template <typename T> void set(const std::string& keyword, const T& value, const std::string& section = "") {
        std::ostringstream o;
        if(!(o << value)) setVar(keyword, "", section);
        else setVar(keyword, o.str(), section);
    }
#endif

    // returns const char* if found c_str() of supplied default_value else. Remark that char* is only valid as long as keyword exists!
    const char* getCString(const std::string& keyword, const char* default_value = NULL, const std::string& section = "");

#ifndef HAVE_SSTREAM
    // returns string if found, supplied default_value else
    std::string getString(const std::string& keyword, const std::string& default_value = "", const std::string& section = "");
    // returns integer if found, supplied default_value else
    int    getInt(const std::string& keyword, const std::string& default_value = "", const std::string& section = "");
    // returns long if found, supplied default_value else
    long getLong(const std::string& keyword, const std::string& default_value = "", const std::string& section = "");
    // returns float if found, supplied default_value else
    float getFloat(const std::string& keyword, const std::string& default_value = "", const std::string& section = "");
    // returns double if found, supplied default_value else
    double getDouble(const std::string& keyword, const std::string& default_value = "", const std::string& section = "");

    // sets integer value of keyword (internally converted to string)
    void setInt(const std::string& keyword, int value, const std::string& section = "");
    // sets long value of keyword (internally converted to string)
    void setLong(const std::string& keyword, long value, const std::string& section = "");
    // sets float value of keyword (internally converted to string)
    void setFloat(const std::string& keyword, float value, const std::string& section = "");
    // sets double value of keyword (internally converted to string)
    void setDouble(const std::string& keyword, double value, const std::string& section = "");
    // sets string value of keyword
    void setString(const std::string& keyword, const std::string& value, const std::string& section = "");
#endif

    // adds a section to the configuration (if it does not yet exist)
    void addSection(const std::string& section, const std::string& comment = "");
    // deletes a section (if it exists)
    void delSection(const std::string& section);

    // displays current configuration content using cout
    void debug();


private:
    struct ConfigEntry {
        std::string keyword;
        std::string value;
        std::string comment;
    };

    struct ConfigSection {
        std::string name;
        std::string comment;
        std::list<ConfigEntry> entries;
    };

    enum {
        SL_NORMAL,
        SL_ESCAPE,
        SL_SAFEMODE,
        SL_SAFEESCAPE,
        SL_COMMENT
    } SPLIT_LINE_STATE;

    std::string filename;
    bool pformat;
    std::list<ConfigSection> sections;

    // splits line into tokens and stores them in ret. Supports delimiters, escape characters, ignores special characters between safemode_char and between comment_char and linend '\n'.
    // returns SPLIT_LINE_STATE the parser was in when returning
    int split_line(std::vector<std::string>& ret, std::string& line, const std::string& delimiters = " \t\r\n", char escape_char = '\\', char safemode_char = '"', char comment_char = '\0', int start_state = SL_NORMAL);

    // escapes special characters (\n, \t, \v, \b, \r, \f, \a, \?) found in line and stores the escaped version in esc_line. Additional characters to escape may be given in escape_chars.
    void escape_line(std::string& esc_line, std::string& line, std::string escape_chars = "");
    // searches section and returns an iterator to the section
    // return 1 if section was found, 0 else
    int findSection(const std::string& section, std::list<ConfigSection>::iterator& it);
    // finds an entry in given section and stores iterator in it
    // return 1 if found, 0 else
    int findEntry(const std::string& keyword, const std::string& section, std::list<ConfigEntry>::iterator& it);
};

#endif // _CFG_PARSER_H

