cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_ZLIB zlib)

# Path to source files
set(ZLIB_DIR ${CORE_DIR}/DesktopEditor/cximage/zlib)
message(STATUS "Zlib source path: ${ZLIB_DIR}")

# Library source .h .cpp
file(GLOB ZLIB_C ${ZLIB_DIR}/*.c)

# Exclude sources
list(REMOVE_ITEM ZLIB_C
    ${ZLIB_DIR}/example.c
)

# Set targer as static library
add_library(${LIB_NAME_ZLIB}
    STATIC
        ${ZLIB_C}
)

# Add include files .h
target_include_directories(${LIB_NAME_ZLIB}
    PUBLIC
        ${ZLIB_DIR}
)