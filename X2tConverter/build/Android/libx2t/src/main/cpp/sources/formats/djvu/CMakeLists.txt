cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

if (NOT DEFINED WORKAROUND_DIR)
    message(FATAL_ERROR "You must set path in \"WORKAROUND_DIR\" to \"workaround\" source!")
endif()

# Name for library
set(LIB_NAME_DJVU djvu)

# Path to source files
set(DJVU_DIR ${CORE_DIR}/DjVuFile)
message(STATUS "DjVu source path: ${DJVU_DIR}")

# Library source .h .cpp
file(GLOB DJVU_CPP
    ${WORKAROUND_DIR}/mblen/mblen.c
    ${DJVU_DIR}/DjVu.cpp
    ${DJVU_DIR}/DjVuFileImplementation.cpp
    ${DJVU_DIR}/libdjvu/*.cpp
)

# Set targer as static library
add_library(${LIB_NAME_DJVU}
    STATIC
        ${DJVU_CPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_DJVU}
    PRIVATE
        ${WORKAROUND_DIR}/mblen
)

# Set target definition
target_compile_definitions(${LIB_NAME_DJVU}
    PRIVATE
        _UNICODE
        UNICODE
        HAVE_UNISTD_H
        HAVE_MBSTATE_T
        GCONTAINER_NO_MEMBER_TEMPLATES=1
        HAS_WCHAR
        HAVE_WCHAR_H
        UNIX
        HAVE_STDINCLUDES
        DJVU_USE_DYNAMIC_LIBRARY
)