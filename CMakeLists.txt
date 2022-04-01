# minimum version of cmake
cmake_minimum_required (VERSION 3.1)

# project name
project (AutoGenSECYAN)

#set (CMAKE_CXX_STANDARD 11)
#set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -lstdc++ -Wall")

set(INC_DIR ../sql-parser/src/)
set(LINK_DIR ../sql-parser/ /usr/local/lib)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})

add_subdirectory(SECYAN)
add_subdirectory(autogen)

