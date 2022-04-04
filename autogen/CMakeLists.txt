set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -lstdc++")

set(INC_DIR ../sql-parser/src/ ../SECYAN/src)
set(LINK_DIR ../sql-parser/ /usr/local/lib)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})

aux_source_directory(. DIR_SRCS)
add_library (common ${DIR_SRCS})

find_library(RUNTIME_LIB sqlparser ${LINK_DIR})
link_libraries(sqlparser)

add_executable(main autogen.cpp)

target_link_libraries(common INTERFACE
        PUBLIC secyan
        ABY::aby
        libOTe
        relic_s
        )

target_include_directories(common PUBLIC
        ../SECYAN/extern/ABY/src/abycore
        ../SECYAN/extern/ABY/extern/ENCRYPTO_utils/src
        ../SECYAN/extern/libOTe/cryptoTools
        ../SECYAN/extern/libOTe/
        )

target_link_libraries(main 
    PUBLIC secyan 
    common 
    sqlparser)
