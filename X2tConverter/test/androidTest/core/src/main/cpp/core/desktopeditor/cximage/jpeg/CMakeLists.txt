cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED JPEG_DIR)
    message(FATAL_ERROR "You must set path in \"JPEG_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_JPEG)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_JPEG\"!")
endif()

# Library source .h .cpp
file(GLOB JPEG_C ${JPEG_DIR}*.c)

# Exclude sources
list(REMOVE_ITEM JPEG_C
    ${JPEG_DIR}jmemdos.c
    ${JPEG_DIR}jmemmac.c
    ${JPEG_DIR}jmemnobs.c
    ${JPEG_DIR}jmemname.c
    ${JPEG_DIR}example.c
)

# Set targer as static library
add_library(${LIB_NAME_JPEG} STATIC ${JPEG_C})

# Add include files .h
target_include_directories(${LIB_NAME_JPEG}
    PUBLIC
        ${JPEG_DIR}
)

# Add compile options
target_compile_options(${LIB_NAME_JPEG}
    PUBLIC
        -Wno-format-security
)