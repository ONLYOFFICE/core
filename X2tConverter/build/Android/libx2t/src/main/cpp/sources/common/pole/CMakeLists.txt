cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_POLE pole)

# Path to source files
set(POLE_DIR ${CORE_DIR}/Common/3dParty/pole)
message(STATUS "Pole source path: ${POLE_DIR}")

# Library source .h .cpp
file(GLOB POLE_CPP ${POLE_DIR}/pole.cpp)

# Set targer as static library
add_library(${LIB_NAME_POLE}
    STATIC
        ${POLE_CPP}
)