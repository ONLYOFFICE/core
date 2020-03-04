cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_FONT_ENGINE font-engine)

# Path to source files
set(FONT_ENGINE_DIR ${CORE_DIR}/DesktopEditor/fontengine)
message(STATUS "Font engine source path: ${FONT_ENGINE_DIR}")

# Engine library source .h .cpp
file(GLOB FONT_ENGINE_CPP
    ${FONT_ENGINE_DIR}/*.cpp
    ${FONT_ENGINE_DIR}/fontconverter/*.cpp
)

# Set no optimization
set_release_oflags(-O0)
set_debug_oflags(-O0)

# Set targer as static library
add_library(${LIB_NAME_FONT_ENGINE}
    STATIC
        ${FONT_ENGINE_CPP}
)

# Add dependency library
target_link_libraries(${LIB_NAME_FONT_ENGINE}
    PUBLIC
        graphics
        freetype
        agg
)

# Add include files .h
target_include_directories(${LIB_NAME_FONT_ENGINE}
    PUBLIC
        ${FONT_ENGINE_DIR}
    PRIVATE
        ${FONT_ENGINE_DIR}/fontconverter
)

# Add definisions
target_compile_definitions(${LIB_NAME_FONT_ENGINE}
    PRIVATE
        ONLYOFFICE_FONTS_VERSION_=1
)