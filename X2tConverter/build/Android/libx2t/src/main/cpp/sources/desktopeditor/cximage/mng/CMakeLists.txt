cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_MNG mng)

# Path to source files
set(MNG_DIR ${CORE_DIR}/DesktopEditor/cximage/mng)
message(STATUS "Mng source path: ${MNG_DIR}")

# Library source .h .cpp
file(GLOB MNG_C ${MNG_DIR}/*.c)

# Set targer as static library
add_library(${LIB_NAME_MNG}
    STATIC
        ${MNG_C}
)

# Add include files .h
target_include_directories(${LIB_NAME_MNG}
    PUBLIC
        ${MNG_DIR}
)