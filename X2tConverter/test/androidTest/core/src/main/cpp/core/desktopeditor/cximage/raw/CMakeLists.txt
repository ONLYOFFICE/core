cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED WORKAROUND_DIR)
    message(FATAL_ERROR "You must set argument \"WORKAROUND_DIR\"")
endif()

# Lib src path
if (NOT DEFINED RAW_DIR)
    message(FATAL_ERROR "You must set path in \"RAW_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_RAW)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_RAW\"!")
endif()

# Library source .h .cpp
file(GLOB RAW_C ${RAW_DIR}*.c)

# Exclude sources
list(REMOVE_ITEM RAW_C
    ${RAW_DIR}dcraw.c
)

# Set targer as static library
add_library(${LIB_NAME_RAW} STATIC ${RAW_C})

# Add include files .h
target_include_directories(${LIB_NAME_RAW}
    PUBLIC
        ${RAW_DIR}
)

# Set target definition
target_compile_definitions(${LIB_NAME_RAW}
    PRIVATE _LINUX
)