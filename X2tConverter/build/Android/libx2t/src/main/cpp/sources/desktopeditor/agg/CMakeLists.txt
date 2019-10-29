cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_AGG agg)

# Path to source files
set(AGG_DIR ${CORE_DIR}/DesktopEditor/agg-2.4)
message(STATUS "Agg source path: ${AGG_DIR}")

# Library source .h .cpp
file(GLOB AGG_CPP ${AGG_DIR}/src/*.cpp)

# Set targer as static library
add_library(${LIB_NAME_AGG}
    STATIC
        ${AGG_CPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_AGG}
    PUBLIC
        ${AGG_DIR}/include/
)