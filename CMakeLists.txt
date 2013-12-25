set(PROJECT_NAME gbook)
project(${PROJECT_NAME})

cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")

add_executable(${PROJECT_NAME} src/main.cpp src/screen.cpp src/window.cpp src/event_loop.cpp)

SET(CURSES_NEED_NCURSES TRUE)
find_package(Curses)
include_directories(${CURSES_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})

install(TARGETS gbook RUNTIME DESTINATION bin)
