cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED DJVU_DIR)
    message(FATAL_ERROR "You must set path in \"DJVU_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_DJVU)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_DJVU\"!")
endif()

# Library source .h .cpp
file(GLOB DJVU_CPP
    ${WORKAROUND_DIR}/mblen/mblen.c
    ${DJVU_DIR}DjVu.cpp
    ${DJVU_DIR}DjVuFileImplementation.cpp
    ${DJVU_DIR}libdjvu/*.cpp
)

# Set targer as static library
add_library(${LIB_NAME_DJVU} STATIC ${DJVU_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_DJVU}
    PUBLIC
        ${LIB_NAME_FONT_ENGINE}
)

# Add include files .h
target_include_directories(${LIB_NAME_DJVU}
    PUBLIC
        ${DJVU_DIR}
    PRIVATE
        ${DJVU_DIR}libdjvu/
        ${WORKAROUND_DIR}/mblen/
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

# Set target definition
target_compile_options(${LIB_NAME_DJVU}
    PRIVATE
        -Wno-c++11-narrowing
        -Wno-format-security
        -Wno-register
)