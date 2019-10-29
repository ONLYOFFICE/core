cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_PSD psd)

# Path to source files
set(PSD_DIR ${CORE_DIR}/DesktopEditor/cximage/libpsd)
message(STATUS "Psd source path: ${PSD_DIR}")

# Library source .h .cpp
file(GLOB PSD_C ${PSD_DIR}/*.c)

# Set targer as static library
add_library(${LIB_NAME_PSD}
    STATIC
        ${PSD_C}
)

# Add include files .h
target_include_directories(${LIB_NAME_PSD}
    PUBLIC
        ${PSD_DIR}
)