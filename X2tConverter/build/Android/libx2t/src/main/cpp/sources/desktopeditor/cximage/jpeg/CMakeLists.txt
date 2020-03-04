cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_JPEG jpeg)

# Path to source files
set(JPEG_DIR ${CORE_DIR}/DesktopEditor/cximage/jpeg)
message(STATUS "Jpeg source path: ${JPEG_DIR}")

# Library source .h .cpp
file(GLOB JPEG_C ${JPEG_DIR}/*.c)

# Exclude sources
list(REMOVE_ITEM JPEG_C
    ${JPEG_DIR}/jmemdos.c
    ${JPEG_DIR}/jmemmac.c
    ${JPEG_DIR}/jmemnobs.c
    ${JPEG_DIR}/jmemname.c
    ${JPEG_DIR}/example.c
)

# Set targer as static library
add_library(${LIB_NAME_JPEG}
    STATIC
        ${JPEG_C}
)

# Add include files .h
target_include_directories(${LIB_NAME_JPEG}
    PUBLIC
        ${JPEG_DIR}
)