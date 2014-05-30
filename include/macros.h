#ifndef GBOOK_MACROS_H
#define GBOOK_MACROS_H

#include "logger.h"
#include <sstream>

#define LOG(SEVERITY, MESSAGE)                  \
  jstation::logger_collection::instance().log(  \
    SEVERITY,                                   \
    static_cast<std::ostringstream&>(           \
      std::ostringstream().flush() << MESSAGE   \
    ).str(),                                    \
    __FUNCTION__,                               \
    __FILE__,                                   \
    __LINE__                                    \
  );

#define LOG_ERROR(MESSAGE)                      \
  jstation::logger_collection::instance().log(  \
    jstation::severity::ERROR,                  \
    static_cast<std::ostringstream&>(           \
      std::ostringstream().flush() << MESSAGE   \
    ).str(),                                    \
    __FUNCTION__,                               \
    __FILE__,                                   \
    __LINE__                                    \
  );

#define LOG_WARNING(MESSAGE)                    \
  jstation::logger_collection::instance().log(  \
    jstation::severity::WARNING,                \
    static_cast<std::ostringstream&>(           \
      std::ostringstream().flush() << MESSAGE   \
    ).str(),                                    \
    __FUNCTION__,                               \
    __FILE__,                                   \
    __LINE__                                    \
  );

#define LOG_INFO(MESSAGE)                       \
  jstation::logger_collection::instance().log(  \
    jstation::severity::INFO,                   \
    static_cast<std::ostringstream&>(           \
      std::ostringstream().flush() << MESSAGE   \
    ).str(),                                    \
    "",                                         \
    "",                                         \
     0                                          \
  );

#define LOG_DEBUG(MESSAGE)                      \
  jstation::logger_collection::instance().log(  \
    jstation::severity::DEBUG,                  \
    static_cast<std::ostringstream&>(           \
      std::ostringstream().flush() << MESSAGE   \
    ).str(),                                    \
    __FUNCTION__,                               \
    __FILE__,                                   \
    __LINE__                                    \
  );

#define LOG_DEBUG1(MESSAGE)                     \
  jstation::logger_collection::instance().log(  \
    jstation::severity::DEBUG1,                 \
    static_cast<std::ostringstream&>(           \
      std::ostringstream().flush() << MESSAGE   \
    ).str(),                                    \
    __FUNCTION__,                               \
    __FILE__,                                   \
    __LINE__                                    \
  );

#define LOG_DEBUG2(MESSAGE)                     \
  jstation::logger_collection::instance().log(  \
    jstation::severity::DEBUG2,                 \
    static_cast<std::ostringstream&>(           \
      std::ostringstream().flush() << MESSAGE   \
    ).str(),                                    \
    __FUNCTION__,                               \
    __FILE__,                                   \
    __LINE__                                    \
  );

#define LOG_DEBUG3(MESSAGE)                     \
  jstation::logger_collection::instance().log(  \
    jstation::severity::DEBUG3,                 \
    static_cast<std::ostringstream&>(           \
      std::ostringstream().flush() << MESSAGE   \
    ).str(),                                    \
    __FUNCTION__,                               \
    __FILE__,                                   \
    __LINE__                                    \
  );

#endif // GBOOK_MACROS_H