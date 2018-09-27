cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED PNG_DIR)
    message(FATAL_ERROR "You must set path in \"PNG_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_PNG)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_PNG\"!")
endif()

# Library source .h .cpp
file(GLOB PNG_C ${PNG_DIR}*.c)

# Exclude sources
list(REMOVE_ITEM PNG_C
    ${PNG_DIR}pngvalid.c
    ${PNG_DIR}example.c
)

# Set targer as static library
add_library(${LIB_NAME_PNG} STATIC ${PNG_C})

# Add include files .h
target_include_directories(${LIB_NAME_PNG}
    PUBLIC
        ${PNG_DIR}
)