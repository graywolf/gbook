#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <libgen.h>
#include <iomanip>

namespace jstation {
    enum class severity {
        ERROR = 0, WARNING = 1, INFO = 2, DEBUG = 3, DEBUG1 = 4, DEBUG2 = 5, DEBUG3 = 6
    };

    class logger {
    public:
        virtual ~logger() {}
        virtual void set_threshold(severity) =0;
        virtual void operator()(
            severity,
            std::string message,
            std::string file,
            std::string function,
            int line
        ) =0;
    protected:
        std::string to_string(severity s) {
            switch (s) {
                case severity::ERROR:
                    return "ERROR  ";
                case severity::WARNING:
                    return "WARNING";
                case severity::INFO:
                    return "INFO   ";
                case severity::DEBUG:
                    return "DEBUG  ";
                case severity::DEBUG1:
                    return "DEBUG1 ";
                case severity::DEBUG2:
                    return "DEBUG2 ";
                case severity::DEBUG3:
                    return "DEBUG3 ";
            }
            return "";
        }
    };

    class cout_logger : public logger {
    public:
        ~cout_logger() {}
        void operator()(
            severity s,
            std::string message,
            std::string function,
            std::string file,
            int line
        ) {
            if (s <= threshold_) {
                char timestamp[20];
                time_t raw_time;
                time(&raw_time);
                struct tm * timeinfo = localtime(&raw_time);
                strftime(timestamp, 20, "%T", timeinfo);
                std::cout   << timestamp
                            << ":"
                            << to_string(s)
                            << ": "
                            << std::left << std::setw(14) << file_path_to_name(file)
                            << " : "
                            << message;
                if (!file.empty()) {
                    std::cout << " in " << file << ":" << function << ":" << line;
                }
                std::cout << std::endl;
            }
        }

        void set_threshold(severity threshold) {
            threshold_ = threshold;
        }
    private:
        std::string file_path_to_name(std::string path) {
            char * f = new char[path.size() + 1];
            copy(path.begin(), path.end(), f);
            f[path.size()] = 0x00;
            char * bname = basename(f);
            path = std::string(bname);
            path = path.substr(0, path.find('.'));
            delete f;
            return path;
        }
        severity threshold_= severity::WARNING;
    };

    class logger_collection {
    public:

        ~logger_collection() {
            for (auto o : outs_) {
                if (o.owner) {
                    delete o.log;
                }
            }
        }

        static logger_collection & instance() {
            static logger_collection lc;
            return lc;
        }

        void log(
            severity s,
            std::string message,
            std::string file,
            std::string function,
            int line
        ) {
            for (auto o : outs_) {
                (*o.log)(
                    s,
                    message,
                    file,
                    function,
                    line
                );
            }
        }

        void add_logger(logger * l, bool owner = true) {
            outs_.push_back(out(l, owner));
        }
    private:
        logger_collection() {}
        struct out {
            out(logger * l, bool owner) : log(l), owner(owner) {}
            logger * log;
            bool owner;
        };

        std::vector<out> outs_;
    };
}

#endif // LOGGER_H
