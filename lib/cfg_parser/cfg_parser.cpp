//
//  cfg_parser.cpp
//  Config File Parser
//
//  Created by Clemens Wacha
//  Version 1.0
//  Copyright (c) 2006 Clemens Wacha. All rights reserved
//

#include "cfg_parser.h"
#include <stdlib.h>
#include <iostream>

#define DEBUG

#ifdef DEBUG
    #include <stdio.h>
    #define ErrorMessage(x...) printf(x)
    #define DebugMessage(x...) printf(x)
#else
    #define ErrorMessage(x)
#endif


using namespace std;

ConfigParser::ConfigParser(const std::string& in_file) {
    filename = in_file;
    pformat = 0;

    if(in_file != "") {
        readFile(in_file);
    }
}

ConfigParser::~ConfigParser() {}

int ConfigParser::readFile(const std::string& in_file) {
    FILE* fd = NULL;
    char line[CFG_MAXLENGTH];
    unsigned int linenumber = 0;
    vector<string> tokens;
    string keyword, value, comment, section;
    unsigned int pos;
    int state = SL_NORMAL;
    string tmp;

    fd = fopen(in_file.c_str(), "rt");
    if(!fd) {
        ErrorMessage("ConfigParser: Could not open %s\n", filename.c_str());
        return 0;
    }

    while(!feof(fd)) {
        fgets(line, sizeof(line), fd);
        tmp += line;
        if(tmp.size() > 0 && tmp[tmp.size()-1] != '\n') continue;

        linenumber++;

        state = split_line(tokens, tmp, " =\t\r\n", '\\', '"', '#', state);
        if(state != SL_NORMAL) {
            // multiline option
            tmp = "";
            continue;
        }

        // remove line endings from tmp (to have nice error message output)
        pos = tmp.rfind('\n');
        if(pos != string::npos) tmp.erase(pos);
        pos = tmp.rfind('\r');    // windows linebreak :-(
        if(pos != string::npos) tmp.erase(pos);


        // first, see if there is a comment token
        if(tokens.size() > 0) {
            if(tokens.back()[0] == '#') {
                // comment found
                comment = tokens.back();
                tokens.pop_back();
            }
        }

        // check if it is a section
        string section_string;
        for(unsigned int i = 0; i < tokens.size(); i++) section_string += tokens[i];
        if(section_string.size() >= 3 && section_string[0] == '[' && section_string[section_string.size()-1] == ']') {
            // its a section
            section = section_string.substr(1, section_string.size()-2);
            addSection(section, comment);
            tokens.clear();
            comment = "";
            tmp = "";
            continue;
        }

        // check if it is a valid "keyword, value" pair
        switch(tokens.size()) {
            case 2:
                keyword = tokens[0];
                value = tokens[1];
                break;
            case 0:
                // the line contained a comment only (or was empty)
                // if we have to preserve the format we add a special keyword always. Otherwise only if comment is not empty
                if(pformat)
                    keyword = "-";
                else
                    if(comment != "") keyword = "-";
                break;
            default:
                // error
                ErrorMessage("ConfigParser:%s:%d: Invalid format: %s\nValid format is 'keyword = value' or 'keyword value'\n", in_file.c_str(), linenumber, tmp.c_str());
                //ErrorMessage("Tokens read: ");
                //for(unsigned int i = 0; i < tokens.size(); i++) ErrorMessage("%s, ", tokens[i].c_str());
                //ErrorMessage("\n");
                break;
        }

        setVar(keyword, value, section, comment);

        tokens.clear();
        keyword = "";
        value = "";
        comment = "";
        tmp = "";
    }
    fclose(fd);
    return 1;
}

int ConfigParser::writeFile(const std::string& out_file) {
    list<ConfigSection>::iterator section = sections.begin();
    list<ConfigEntry>::iterator entry;
    FILE* fd;
    string file_name;
    string esc_value;
    string esc_keyword;

    if(out_file != "") file_name = out_file;
    else file_name = filename;

    fd = fopen(file_name.c_str(), "wt");
    if(!fd) {
        ErrorMessage("ConfigParser: Could not open %s for writing\n", file_name.c_str());
        return 0;
    }

    while(section != sections.end()) {
        if(section->name != "") {
            if(!pformat) fprintf(fd, "\n");
            fprintf(fd, "[%s]", section->name.c_str());
            if(section->comment != "") fprintf(fd, "\t %s\n", section->comment.c_str());
            else fprintf(fd, "\n");
        }

        entry = section->entries.begin();
        while(entry != section->entries.end()) {
            if(entry->keyword == "-") {
                fprintf(fd, "%s\n", entry->comment.c_str());
            } else {
                // be careful with values.
                esc_keyword = "";
                esc_value = "";
                escape_line(esc_keyword, entry->keyword, " \"");
                escape_line(esc_value, entry->value, "\"");
                //if(esc_keyword.size() > entry->keyword.size()) esc_keyword = "\"" + esc_keyword + "\"";
                if(esc_value.size() > entry->value.size()) esc_value = "\"" + esc_value + "\"";

                fprintf(fd, "%s = %s", esc_keyword.c_str(), esc_value.c_str());

                if(entry->comment != "") fprintf(fd, "\t %s\n", entry->comment.c_str());
                else fprintf(fd, "\n");
            }
            entry++;
        }
        section++;
    }

    fclose(fd);
    return 1;
}

void ConfigParser::getFilename(std::string& in_file) {
    in_file = filename;
}

void ConfigParser::preserveEmptyLines(bool enable) {
    pformat = enable;
}

void ConfigParser::getVar(const std::string& keyword, std::string& value, const std::string& section) {
    list<ConfigEntry>::iterator it;

    if( findEntry(keyword, section, it) ) {
        // found
        value = it->value;
    }
}

void ConfigParser::setVar(const std::string& keyword, const std::string& value, const std::string& section, const std::string& comment) {
    std::list<ConfigSection>::iterator it;
    std::list<ConfigEntry>::iterator it2;
    ConfigEntry entry;

    if(keyword == "") return;

    addSection(section);
    findSection(section, it);

    // if keyword is "-" then this variable is actually a comment and will be push_back'ed anyways

    if( keyword == "-" || !findEntry(keyword, section, it2) ) {
        entry.keyword = keyword;
        entry.value = value;
        entry.comment = comment;
        it->entries.push_back(entry);
    } else {
        it2->value = value;
        it2->comment = comment;     //TODO: how to not alter the comment??
    }
}

void ConfigParser::delVar(const std::string& keyword, const std::string& section) {
    std::list<ConfigSection>::iterator sec;
    std::list<ConfigEntry>::iterator entry;

    if(keyword == "" || keyword == "-") return;

    if( !findEntry(keyword, section, entry) ) return;
    findSection(section, sec);

    sec->entries.erase(entry);
}


const char* ConfigParser::getCString(const std::string& keyword, const char* default_value, const std::string& section) {
    list<ConfigEntry>::iterator it;

    if( findEntry(keyword, section, it) ) {
        // found
        return it->value.c_str();
    }
    return default_value;
}

#ifndef HAVE_SSTREAM
std::string ConfigParser::getString(const std::string& keyword, const std::string& default_value, const std::string& section) {
    string value = default_value;
    getVar(keyword, value, section);
    return value;
}


int ConfigParser::getInt(const std::string& keyword, const std::string& default_value, const std::string& section) {
    string value = default_value;
    getVar(keyword, value, section);
    return atoi(value.c_str());
}


long ConfigParser::getLong(const std::string& keyword, const std::string& default_value, const std::string& section) {
    string value = default_value;
    getVar(keyword, value, section);
    return atol(value.c_str());
}

float ConfigParser::getFloat(const std::string& keyword, const std::string& default_value, const std::string& section) {
    string value = default_value;
    getVar(keyword, value, section);
    return atof(value.c_str());
}

double ConfigParser::getDouble(const std::string& keyword, const std::string& default_value, const std::string& section) {
    string value = default_value;
    getVar(keyword, value, section);
    return atof(value.c_str());
}

void ConfigParser::setInt(const std::string& keyword, int value, const std::string& section) {
    char buf[30];
    sprintf(buf, "%d", value);
    setVar(keyword, string(buf), section);
}

void ConfigParser::setLong(const std::string& keyword, long value, const std::string& section) {
    char buf[30];
    sprintf(buf, "%ld", value);
    setVar(keyword, string(buf), section);
}

void ConfigParser::setFloat(const std::string& keyword, float value, const std::string& section) {
    char buf[30];
    sprintf(buf, "%f", value);
    setVar(keyword, string(buf), section);
}

void ConfigParser::setDouble(const std::string& keyword, double value, const std::string& section) {
    char buf[30];
    sprintf(buf, "%f", value);
    setVar(keyword, string(buf), section);
}

void ConfigParser::setString(const std::string& keyword, const std::string& value, const std::string& section) {
    setVar(keyword, value, section);
}
#endif // HAVE_SSTREAM


void ConfigParser::addSection(const std::string& section, const std::string& comment) {
    std::list<ConfigSection>::iterator it;
    ConfigSection s;

    //if(section == "") return;

    if( findSection(section, it) ) return;

    s.name = section;
    s.comment = comment;
    sections.push_back(s);
}

void ConfigParser::delSection(const std::string& section) {
    std::list<ConfigSection>::iterator it;

    if( !findSection(section, it) ) return;
    sections.erase(it);
}

void ConfigParser::debug() {
    std::list<ConfigSection>::iterator section = sections.begin();
    std::list<ConfigEntry>::iterator entry;

    cout << endl;
    cout << "========= Debug Output ============================================" << endl;

    while(section != sections.end()) {
        cout << "Section: [" << section->name << "]" << endl;
        cout << "Comment: '" << section->comment << "'" << endl;
        cout << "==========================================================" << endl;
        entry = section->entries.begin();
        while(entry != section->entries.end()) {
            cout << "Keyword: '" << entry->keyword << "'" << endl;
            cout << "Value: '" << entry->value << "'" << endl;
            cout << "Comment: '" << entry->comment << "'" << endl;
            cout << "---------" << endl;
            entry++;
        }
        cout << endl;
        section++;
    }
    cout << "===================================================================" << endl;
}


int ConfigParser::split_line(vector<string>& ret, string& line, const string& delimiters, char escape_char, char safemode_char, char comment_char, int start_state) {
    unsigned int state = start_state;
    unsigned int i = 0;
    string token;

    if(start_state != SL_NORMAL && ret.size() > 0) {
        token = ret[ret.size()-1];
        ret.pop_back();
    }

    while(i < line.size()) {
        switch(state) {
            case SL_NORMAL:
                if(line[i] == escape_char) {
                    state = SL_ESCAPE;
                } else if(line[i] == safemode_char) {
                    state = SL_SAFEMODE;
                } else if(line[i] == comment_char) {
                    /// FINISH
                    if(token.size() > 0) {
                        ret.push_back(token);
                        token.clear();
                    }
                    token += line[i];    // EAT
                    state = SL_COMMENT;
                } else if(delimiters.find(line[i]) != string::npos) {
                    // line[i] is a delimiter
                    /// FINISH
                    if(token.size() > 0) {
                        ret.push_back(token);
                        token.clear();
                    }
                } else {
                    token += line[i];    // EAT
                }
                break;
            case SL_ESCAPE:
                if(line[i] == 'n') token += '\n';
                else if(line[i] == 't') token += '\t';
                else if(line[i] == 'v') token += '\v';
                else if(line[i] == 'b') token += '\b';
                else if(line[i] == 'r') token += '\r';
                else if(line[i] == 'f') token += '\f';
                else if(line[i] == 'a') token += '\a';
                else if(line[i] == '?') token += '\?';
                else token += line[i];    // EAT
                state = SL_NORMAL;
                break;
            case SL_SAFEMODE:
                if(line[i] == safemode_char) {
                    state = SL_NORMAL;
                } else if(line[i] == escape_char) {
                    state = SL_SAFEESCAPE;
                } else {
                    token += line[i];    // EAT
                }
                break;
            case SL_SAFEESCAPE:
                if(line[i] == 'n') token += '\n';
                else if(line[i] == 't') token += '\t';
                else if(line[i] == 'v') token += '\v';
                else if(line[i] == 'b') token += '\b';
                else if(line[i] == 'r') token += '\r';
                else if(line[i] == 'f') token += '\f';
                else if(line[i] == 'a') token += '\a';
                else if(line[i] == '?') token += '\?';
                else token += line[i];    // EAT
                state = SL_SAFEMODE;
                break;
            case SL_COMMENT:
                if(line[i] == '\n') {
                    /// FINISH
                    if(token.size() > 0) {
                        ret.push_back(token);
                        token.clear();
                    }
                    state = SL_NORMAL;
                } else {
                    token += line[i];    // EAT
                }
                break;
            default:
                // nothing
                break;
        }

        i++;
    }

    /// FINISH
    if(token.size() > 0) {
        ret.push_back(token);
        token.clear();
    }
    return(state);
}

void ConfigParser::escape_line(std::string& esc_line, std::string& line, std::string escape_chars) {
    unsigned int i = 0;

    while( i < line.size() ) {
        switch(line[i]) {
            case '\n':
                esc_line += "\\n";
                break;
            case '\t':
                esc_line += "\\t";
                break;
            case '\v':
                esc_line += "\\v";
                break;
            case '\b':
                esc_line += "\\b";
                break;
            case '\r':
                esc_line += "\\r";
                break;
            case '\f':
                esc_line += "\\f";
                break;
            case '\a':
                esc_line += "\\a";
                break;
            case '\?':
                esc_line += "\\?";
                break;
            default:
                if(escape_chars.find(line[i]) != string::npos) esc_line += "\\";
                esc_line += line[i];
        }
        i++;
    }
}

int ConfigParser::findSection(const std::string& section, std::list<ConfigSection>::iterator& it) {
    it = sections.begin();

    //find section
    while(it != sections.end()) {
        if(it->name == section) return 1;
        it++;
    }

    return 0;
}

int ConfigParser::findEntry(const std::string& keyword, const std::string& section, std::list<ConfigEntry>::iterator& it) {
    std::list<ConfigSection>::iterator sec;
    if( !findSection(section, sec) ) return 0;

    it = sec->entries.begin();

    while(it != sec->entries.end()) {
        if(it->keyword == keyword) return 1;
        it++;
    }
    return 0;
}


