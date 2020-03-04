cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_RAW raw)

# Path to source files
set(RAW_DIR ${CORE_DIR}/DesktopEditor/cximage/raw)
message(STATUS "Raw source path: ${RAW_DIR}")

# Library source .h .cpp
file(GLOB RAW_C ${RAW_DIR}/*.c)

# Exclude sources
list(REMOVE_ITEM RAW_C
    ${RAW_DIR}/dcraw.c
)

# Set targer as static library
add_library(${LIB_NAME_RAW}
    STATIC
        ${RAW_C}
)

# Add include files .h
target_include_directories(${LIB_NAME_RAW}
    PUBLIC
        ${RAW_DIR}
)

# Set target definition
target_compile_definitions(${LIB_NAME_RAW}
    PRIVATE
        _LINUX
)