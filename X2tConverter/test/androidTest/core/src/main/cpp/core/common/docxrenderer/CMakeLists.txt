cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED DOCX_RENDER_DIR)
    message(FATAL_ERROR "You must set path in \"DOCX_RENDER_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_DOCX_RENDER)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_DOCX_RENDER\"!")
endif()

# Library source .h .cpp
file(GLOB DOCX_RENDER_CPP
    ${DOCX_RENDER_DIR}DocxRenderer.cpp
)

# Set targer as static library
add_library(${LIB_NAME_DOCX_RENDER} STATIC ${DOCX_RENDER_CPP})

# Add include files .h
target_include_directories(${LIB_NAME_DOCX_RENDER}
    PUBLIC
        ${DOCX_RENDER_DIR}
        ${DOCX_RENDER_DIR}src/logic/
)

# Set target definition
target_compile_definitions(${LIB_NAME_DOCX_RENDER}
    PRIVATE
        DOCXRENDERER_USE_DYNAMIC_LIBRARY
)