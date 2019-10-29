cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_JASPER jasper)

# Path to source files
set(JASPER_DIR ${CORE_DIR}/DesktopEditor/cximage/jasper)
message(STATUS "Jasper source path: ${JASPER_DIR}")

# Library source .h .cpp
file(GLOB JASPER_C
    ${JASPER_DIR}/base/*.c
    ${JASPER_DIR}/bmp/*.c
    ${JASPER_DIR}/jp2/*.c
    ${JASPER_DIR}/jpc/*.c
    ${JASPER_DIR}/jpg/*.c
    ${JASPER_DIR}/jpg/*.c
    ${JASPER_DIR}/mif/*.c
    ${JASPER_DIR}/pgx/*.c
    ${JASPER_DIR}/pnm/*.c
    ${JASPER_DIR}/ras/*.c
)

# Exclude sources
list(REMOVE_ITEM JASPER_C
    ${JASPER_DIR}/jpg/jpg_dummy.c
)

# Set targer as static library
add_library(${LIB_NAME_JASPER}
    STATIC
        ${JASPER_C}
)

# Add dependency library
target_link_libraries(${LIB_NAME_JASPER}
    PRIVATE
        jpeg
)

# Add include files .h
target_include_directories(${LIB_NAME_JASPER}
    PUBLIC
        ${JASPER_DIR}/include/
        ${JASPER_DIR}/include/jasper/
    PRIVATE
        ${JASPER_DIR}/bmp/
        ${JASPER_DIR}/jp2/
        ${JASPER_DIR}/jpc/
        ${JASPER_DIR}/jpg/
        ${JASPER_DIR}/jpg/
        ${JASPER_DIR}/mif/
        ${JASPER_DIR}/pgx/
        ${JASPER_DIR}/pnm/
        ${JASPER_DIR}/ras/
)

# Set target definition
target_compile_definitions(${LIB_NAME_JASPER}
    PRIVATE
        EXCLUDE_JPG_SUPPORT
        HAVE_UNISTD_H
)

