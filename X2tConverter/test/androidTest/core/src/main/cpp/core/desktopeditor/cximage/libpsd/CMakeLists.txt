cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED PSD_DIR)
    message(FATAL_ERROR "You must set path in \"PSD_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_PSD)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_PSD\"!")
endif()

# Library source .h .cpp
file(GLOB PSD_C ${PSD_DIR}*.c)

# Set targer as static library
add_library(${LIB_NAME_PSD} STATIC ${PSD_C})

# Add include files .h
target_include_directories(${LIB_NAME_PSD}
    PUBLIC
        ${PSD_DIR}
)