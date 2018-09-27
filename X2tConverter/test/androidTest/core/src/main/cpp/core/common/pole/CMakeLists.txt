cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED POLE_DIR)
    message(FATAL_ERROR "You must set path in \"POLE_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_POLE)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_POLE\"!")
endif()

# Library source .h .cpp
file(GLOB POLE_CPP ${POLE_DIR}pole.cpp)

# Set targer as static library
add_library(${LIB_NAME_POLE} STATIC ${POLE_CPP})

# Add include files .h
target_include_directories(${LIB_NAME_POLE}
    PUBLIC
        ${POLE_DIR}
)