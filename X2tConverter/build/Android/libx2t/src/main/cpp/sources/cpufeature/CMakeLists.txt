cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

if (NOT DEFINED ANDROID_NDK)
    message(FATAL_ERROR "You must set path in \"ANDROID_NDK\" to \"NDK\" source!")
endif()

# Name for library
set(LIB_NAME_CPUFEATURE cpufeature)

# Path to source files
set(CPU_FEATURE_DIR ${ANDROID_NDK}/sources/android/cpufeatures)
message(STATUS "CpuFeature source path: ${CPU_FEATURE_DIR}")

# Get list sources
file(GLOB CPUFEATURE_CPP ${CPU_FEATURE_DIR}/*.c)

# Set targer as static library
add_library(${LIB_NAME_CPUFEATURE}
    STATIC
        ${CPUFEATURE_CPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_CPUFEATURE}
    INTERFACE
        ${CPU_FEATURE_DIR}
)