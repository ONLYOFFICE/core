cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_JBIG jbig)

# Path to source files
set(JBIG_DIR ${CORE_DIR}/DesktopEditor/cximage/jbig)
message(STATUS "JBig source path: ${JBIG_DIR}")

# Library source .h .cpp
file(GLOB JBIG_C ${JBIG_DIR}/*.c)

# Exclude sources
list(REMOVE_ITEM JBIG_C ${JBIG_DIR}/tstcodec.c)

# Set targer as static library
add_library(${LIB_NAME_JBIG}
    STATIC
        ${JBIG_C}
)

# Add include files .h
target_include_directories(${LIB_NAME_JBIG}
    PUBLIC
        ${JBIG_DIR}
)