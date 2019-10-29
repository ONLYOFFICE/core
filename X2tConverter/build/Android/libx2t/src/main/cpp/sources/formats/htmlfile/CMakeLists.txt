cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_HTML_FILE html-file)

# Path to source files
set(HTML_FILE_DIR ${CORE_DIR}/HtmlFile)
message(STATUS "Html file source path: ${HTML_FILE_DIR}")

# Library source .h .cpp
file(GLOB HTML_FILE_CPP
    ${HTML_FILE_DIR}/*.cpp
)

# Set targer as static library
add_library(${LIB_NAME_HTML_FILE}
    STATIC
        ${HTML_FILE_CPP}
)

# Set target definition
target_compile_definitions(${LIB_NAME_HTML_FILE}
    PRIVATE
        LINUX
        HTMLFILE_USE_DYNAMIC_LIBRARY
        UNICODECONVERTER_USE_DYNAMIC_LIBRARY
        asc_static_link_libstd
)