cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_UNICODE_CONVERTER unicodeconv)

# Path to source files
set(UNICODE_CONVERTER_DIR ${CORE_DIR}/UnicodeConverter)
message(STATUS "Unicode converter source path: ${UNICODE_CONVERTER_DIR}")


# Library source .h .cpp
file(GLOB UNICODE_CONVERTER_CPP ${UNICODE_CONVERTER_DIR}/*.cpp)

# Set targer as static library
add_library(${LIB_NAME_UNICODE_CONVERTER}
    STATIC
        ${UNICODE_CONVERTER_CPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_UNICODE_CONVERTER}
    PUBLIC
        ${UNICODE_CONVERTER_DIR}
)

# Set target definition
target_compile_definitions(${LIB_NAME_UNICODE_CONVERTER}
    PRIVATE
        UNICODECONVERTER_USE_DYNAMIC_LIBRARY
        U_COMMON_IMPLEMENTATION
        U_I18N_IMPLEMENTATION
        U_IO_IMPLEMENTATION
)