SET(
    PROJECT_NAME
    gbook
)
PROJECT(
    ${PROJECT_NAME}
)

SET(
    CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -g -fno-rtti -Wall -std=c++11"
)
SET(
    PROJECT_FILES
        lib/tinyxml2/tinyxml2.cpp
        src/user_reader.cpp
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
    TinyXML
    REQUIRED
)

CMAKE_MINIMUM_REQUIRED(
    VERSION
        2.8
)

ADD_EXECUTABLE(
    ${PROJECT_NAME}
        src/main.cpp
        ${PROJECT_FILES}
)

ADD_EXECUTABLE(
    test_it
        test/gtest.cpp
        ${PROJECT_FILES}
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
    lib/tinyxml2
    include
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}

    ${CURL_LIBRARY}
    ${JSONCPP_LIBRARIES}
    ${TINYXML_LIBRARIES}
)

TARGET_LINK_LIBRARIES(
    test_it

    gtest
    ${CURL_LIBRARY}
    ${JSONCPP_LIBRARIES}
    ${TINYXML_LIBRARIES}
)


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
        ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/getgit.cmake
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
