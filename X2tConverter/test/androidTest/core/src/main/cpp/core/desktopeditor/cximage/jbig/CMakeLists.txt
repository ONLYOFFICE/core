cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED JBIG_DIR)
    message(FATAL_ERROR "You must set path in \"JBIG_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_JBIG)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_JBIG\"!")
endif()

# Library source .h .cpp
file(GLOB JBIG_C ${JBIG_DIR}*.c)

# Exclude sources
list(REMOVE_ITEM JBIG_C ${JBIG_DIR}tstcodec.c)

# Set targer as static library
add_library(${LIB_NAME_JBIG} STATIC ${JBIG_C})

# Add include files .h
target_include_directories(${LIB_NAME_JBIG}
    PUBLIC
        ${JBIG_DIR}
)