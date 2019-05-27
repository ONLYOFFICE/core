cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED MNG_DIR)
    message(FATAL_ERROR "You must set path in \"MNG_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_MNG)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_MGN\"!")
endif()

# Library source .h .cpp
file(GLOB MNG_C ${MNG_DIR}*.c)

# Set targer as static library
add_library(${LIB_NAME_MNG} STATIC ${MNG_C})

# Add include files .h
target_include_directories(${LIB_NAME_MNG}
    PUBLIC
        ${MNG_DIR}
)