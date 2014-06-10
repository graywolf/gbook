SET(
    PROJECT_NAME
    gbook
)
PROJECT(
    ${PROJECT_NAME}
)

SET(
    CMAKE_CXX_FLAGS
        "-fno-rtti -Wall -std=c++11"
)

SET(
    PROJECT_FILES
        lib/cfg_parser/cfg_parser.cpp
        lib/tinyxml2/tinyxml2.cpp
        src/abook_manager.cpp
        src/contacts.cpp
        src/config.cpp
        src/curl.cpp
        src/google_manager.cpp
        src/merger.cpp
        src/oauth2.cpp
        src/sync.cpp
        src/user_reader.cpp
        src/user_writer.cpp
)
SET(
    CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

FIND_PACKAGE(
    CURL
    REQUIRED
)
FIND_PACKAGE(
    JsonCpp
    REQUIRED
)
FIND_PACKAGE(
    LibConfig
    REQUIRED
)

CMAKE_MINIMUM_REQUIRED(
    VERSION
        2.8
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
    lib
    lib/gtest-1.7.0/include
    lib/gmock-1.7.0/include
    include
)

ADD_EXECUTABLE(
    ${PROJECT_NAME}
        src/main.cpp
        ${PROJECT_FILES}
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}

    ${CURL_LIBRARY}
    ${JSONCPP_LIBRARIES}
    ${LIBCONFIGPP_LIBRARIES}
)

FIND_LIBRARY(
    GTEST_LIBRARY
        gtest lib/gtest-1.7.0/lib/.libs
)
FIND_LIBRARY(
    GMOCK_LIBRARY
        gmock lib/gmock-1.7.0/lib/.libs
)

ADD_EXECUTABLE(
    test_it
        test/gtest.cpp
        ${PROJECT_FILES}
)

TARGET_LINK_LIBRARIES(
    test_it

    ${GTEST_LIBRARY}
    ${GMOCK_LIBRARY}
    ${CURL_LIBRARY}
    ${JSONCPP_LIBRARIES}
)

#ADD_EXECUTABLE(
#    test_google_stuff
#        test/google_stuff.cpp
#        ${PROJECT_FILES}
#

#TARGET_LINK_LIBRARIES(
#    test_google_stuff
#    ${CURL_LIBRARY}
#    ${JSONCPP_LIBRARIES}
#)

# a custom target that is always built
ADD_CUSTOM_TARGET(
    revision_number
    ALL
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

# creates version.h using cmake script, but say that it's config.h to have it rebuild
ADD_CUSTOM_COMMAND(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/config.h
    COMMAND
        ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/getgit.cmake
)

# config.h is considered a generated file
SET_SOURCE_FILES_PROPERTIES(
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    PROPERTIES GENERATED
        TRUE
    HEADER_FILE_ONLY
        TRUE
)

# explicitly say that the executable depends on the svnheader
ADD_DEPENDENCIES(
    ${PROJECT_NAME}
    revision_number
)

install(
    TARGETS
        gbook
    RUNTIME DESTINATION
        bin
)
