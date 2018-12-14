cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED TIFF_DIR)
    message(FATAL_ERROR "You must set path in \"TIFF_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_TIFF)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_TIFF\"!")
endif()

# Library source .h .cpp
file(GLOB TIFF_C ${TIFF_DIR}*.c)

# Exclude sources
list(REMOVE_ITEM TIFF_C
    ${TIFF_DIR}tif_apple.c
    ${TIFF_DIR}tif_acorn.c
    ${TIFF_DIR}tif_atari.c
    ${TIFF_DIR}tif_msdos.c
    ${TIFF_DIR}tif_win3.c
    ${TIFF_DIR}tif_win32.c
)

# Set targer as static library
add_library(${LIB_NAME_TIFF} STATIC ${TIFF_C})

# Add include files .h
target_include_directories(${LIB_NAME_TIFF}
    PUBLIC
        ${TIFF_DIR}
)

# Set target definition
target_compile_definitions(${LIB_NAME_TIFF}
    PRIVATE
        _LINUX
)