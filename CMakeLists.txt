# minimum version of cmake
cmake_minimum_required (VERSION 3.1)

# project name
project (AutoGenSECYAN)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -lstdc++ -Wall -I../sql-parser/src/ -L../sql-parser/")

aux_source_directory(. DIR_SRCS)
add_library (common ${DIR_SRCS})

add_executable(main autogen.cpp)
target_link_libraries(main common sqlparser)