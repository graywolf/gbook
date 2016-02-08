SET(
	PROJECT_NAME
		gbook
)
PROJECT(
	${PROJECT_NAME}
)

option(BUILD_TESTS "Build test programs" OFF)

SET(
	PROJECT_FILES
		lib/cfg_parser/cfg_parser.cpp
		lib/tinyxml2/tinyxml2.cpp
		src/abook_manager.cpp
		src/contacts.cpp
		src/config.cpp
		src/curl.cpp
		src/funcs.cpp
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

CMAKE_MINIMUM_REQUIRED(
	VERSION
		3.0
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/lib
	${CMAKE_CURRENT_BINARY_DIR}
)

ADD_EXECUTABLE(
	${PROJECT_NAME}
		src/main.cpp
		${PROJECT_FILES}
)

TARGET_COMPILE_FEATURES(
	${PROJECT_NAME} PUBLIC
		cxx_range_for
)

TARGET_LINK_LIBRARIES(
	${PROJECT_NAME}
		${CURL_LIBRARY}
		${JSONCPP_LIBRARIES}
)

if(BUILD_TESTS)
	ADD_SUBDIRECTORY(
		lib/gmock-1.7.0
	)

	INCLUDE_DIRECTORIES(
		${gtest_SOURCE_DIR}/include
		${gmock_SOURCE_DIR}/include
	)


	ADD_EXECUTABLE(
		test_it
			test/gtest.cpp
			${PROJECT_FILES}
	)

	TARGET_COMPILE_FEATURES(
		test_it PUBLIC
			cxx_range_for
	)

	TARGET_LINK_LIBRARIES(
		test_it
			gmock_main
			${CURL_LIBRARY}
			${JSONCPP_LIBRARIES}
	)
endif(BUILD_TESTS)

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

# explicitly say that the executable depends on the revision number
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
