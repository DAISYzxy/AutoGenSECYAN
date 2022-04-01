# minimum version of cmake
cmake_minimum_required (VERSION 3.1)

# project name
project (AutoGenSECYAN)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -lstdc++ -Wall")

set(INC_DIR ../sql-parser/src/)
set(LINK_DIR ../sql-parser/ /usr/local/lib)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})

aux_source_directory(. DIR_SRCS)
add_library (common ${DIR_SRCS})

find_library(RUNTIME_LIB sqlparser ${LINK_DIR})
link_libraries(sqlparser)

add_executable(main autogen.cpp)
target_link_libraries(main common sqlparser)