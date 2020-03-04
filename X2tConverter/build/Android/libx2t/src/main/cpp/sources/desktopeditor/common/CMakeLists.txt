cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_EDITOR_COMMON editor-common)

# Path to source files
set(COMMON_EDITOR_DIR ${CORE_DIR}/DesktopEditor/common)
message(STATUS "Common editor source path: ${COMMON_EDITOR_DIR}")

# Library source .h .cpp
file(GLOB COMMON_EDITOR_CPP ${COMMON_EDITOR_DIR}/*.cpp)

# Set targer as static library
add_library(${LIB_NAME_EDITOR_COMMON}
    STATIC
        ${COMMON_EDITOR_CPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_EDITOR_COMMON}
    PUBLIC
        ${COMMON_EDITOR_DIR}
)

# Set target definition
target_compile_definitions(${LIB_NAME_EDITOR_COMMON}
    PRIVATE
        _LINUX
)