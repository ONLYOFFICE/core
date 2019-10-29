cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_PNG png)

# Path to source files
set(PNG_DIR ${CORE_DIR}/DesktopEditor/cximage/png)
message(STATUS "Png source path: ${PNG_DIR}")

# Library source .h .cpp
file(GLOB PNG_C ${PNG_DIR}/*.c)

# Exclude sources
list(REMOVE_ITEM PNG_C
    ${PNG_DIR}/pngvalid.c
    ${PNG_DIR}/example.c
)

# Set targer as static library
add_library(${LIB_NAME_PNG}
    STATIC
        ${PNG_C}
)

# Add include files .h
target_include_directories(${LIB_NAME_PNG}
    PUBLIC
        ${PNG_DIR}
)