cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CPU_FEATURE_DIR)
    message(FATAL_ERROR "You must set path in \"CPU_FEATURE_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_CPUFEATURE)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_CPUFEATURE\"!")
endif()

# Library source .h .cpp
file(GLOB CPUFEATURE_CPP ${CPU_FEATURE_DIR}/*.c)

# Set targer as static library
add_library(${LIB_NAME_CPUFEATURE} STATIC ${CPUFEATURE_CPP})

# Add include files .h
target_include_directories(${LIB_NAME_CPUFEATURE} INTERFACE ${CPU_FEATURE_DIR})