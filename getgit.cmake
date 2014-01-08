INCLUDE(
    FindGit
    REQUIRED
)

# get git revision
MACRO(
    GIT_WC_INFO
    dir
)
    EXECUTE_PROCESS(
        COMMAND
            ${GIT_EXECUTABLE} rev-parse --verify -q --short=7 HEAD
        WORKING_DIRECTORY
            ${dir}
        OUTPUT_VARIABLE
            GIT_WC_REVISION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endmacro(
    GIT_WC_INFO
)

GIT_WC_INFO(
    .
)

CONFIGURE_FILE(
    ${SOURCE_DIR}/include/version.h.in
    ${CMAKE_BINARY_DIR}/version.h
)
