SET(
    PROJECT_NAME
    gbook
)

PROJECT(
    ${PROJECT_NAME}
)

CMAKE_MINIMUM_REQUIRED(
    VERSION
        2.8
)

SET(
    CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11"
)

ADD_EXECUTABLE(
    ${PROJECT_NAME}
        src/main.cpp
)

FIND_PACKAGE(
    Boost 1.55
    COMPONENTS
        program_options
    REQUIRED
)

INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIR}
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}

    ${Boost_LIBRARIES}
)

# include the output directory, where the version.h file is generated
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
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
