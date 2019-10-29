cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_DOCX_RENDER docx-render)

# Path to source files
set(DOCX_RENDER_DIR ${CORE_DIR}/DocxRenderer)
message(STATUS "Docx render source path: ${DOCX_RENDER_DIR}")

# Library source .h .cpp
file(GLOB DOCX_RENDER_CPP
    ${DOCX_RENDER_DIR}/DocxRenderer.cpp
)

# Set targer as static library
add_library(${LIB_NAME_DOCX_RENDER}
    STATIC
        ${DOCX_RENDER_CPP}
)

# Set target definition
target_compile_definitions(${LIB_NAME_DOCX_RENDER}
    PRIVATE
        DOCXRENDERER_USE_DYNAMIC_LIBRARY
)