cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_HTML_RENDER html-render)

# Path to source files
set(HTML_RENDER_DIR ${CORE_DIR}/HtmlRenderer)
message(STATUS "Html draw source path: ${HTML_RENDER_DIR}")

# Library source .h .cpp
file(GLOB HTML_RENDER_CPP
    ${HTML_RENDER_DIR}/src/HTMLRenderer3.cpp
    ${HTML_RENDER_DIR}/src/ASCSVGWriter.cpp
)

# Set targer as static library
add_library(${LIB_NAME_HTML_RENDER}
    STATIC
        ${HTML_RENDER_CPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_HTML_RENDER}
    PUBLIC
        ${HTML_RENDER_DIR}/include/
    PRIVATE
        ${HTML_RENDER_DIR}/src/
)

# Set target definition
target_compile_definitions(${LIB_NAME_HTML_RENDER}
    PRIVATE
        HTMLRENDERER_USE_DYNAMIC_LIBRARY
)