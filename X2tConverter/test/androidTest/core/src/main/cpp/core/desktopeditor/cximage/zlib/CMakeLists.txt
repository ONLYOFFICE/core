cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED ZLIB_DIR)
    message(FATAL_ERROR "You must set path in \"ZLIB_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_ZLIB)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_ZLIB\"!")
endif()

# Library source .h .cpp
file(GLOB ZLIB_C ${ZLIB_DIR}*.c)

# Exclude sources
list(REMOVE_ITEM ZLIB_C
    ${ZLIB_DIR}example.c
)

# Set targer as static library
add_library(${LIB_NAME_ZLIB} STATIC ${ZLIB_C})

# Add include files .h
target_include_directories(${LIB_NAME_ZLIB}
    PUBLIC
        ${ZLIB_DIR}
)