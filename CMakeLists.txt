cmake_minimum_required(VERSION 3.16)
project(komiss)

set(CMAKE_CXX_STANDARD 17)
find_package( Curses REQUIRED )
include_directories( ${CURSES_INCLUDE_DIRS} )

add_executable(komiss main.cpp)
target_link_libraries( komiss ${CURSES_LIBRARIES} )
