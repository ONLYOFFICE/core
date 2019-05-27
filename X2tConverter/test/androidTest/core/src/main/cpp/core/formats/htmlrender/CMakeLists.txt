cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED HTML_RENDER_DIR)
    message(FATAL_ERROR "You must set path in \"HTML_RENDER_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_HTML_RENDER)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_HTML_RENDER\"!")
endif()

# Library source .h .cpp
file(GLOB HTML_RENDER_CPP
    ${HTML_RENDER_DIR}src/HTMLRenderer3.cpp
    ${HTML_RENDER_DIR}src/ASCSVGWriter.cpp
)

# Set targer as static library
add_library(${LIB_NAME_HTML_RENDER} STATIC ${HTML_RENDER_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_HTML_RENDER}
    PUBLIC
        ${LIB_NAME_FONT_ENGINE}
)

# Add include files .h
target_include_directories(${LIB_NAME_HTML_RENDER}
    PRIVATE
        ${HTML_RENDER_DIR}src/
        ${HTML_RENDER_DIR}include/
)

# Set target definition
target_compile_definitions(${LIB_NAME_HTML_RENDER}
    PRIVATE
        HTMLRENDERER_USE_DYNAMIC_LIBRARY
)

# Set target definition
target_compile_options(${LIB_NAME_HTML_RENDER}
    PRIVATE
        -Wno-register
)