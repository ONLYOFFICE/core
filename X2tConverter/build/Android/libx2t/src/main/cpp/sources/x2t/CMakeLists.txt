cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_X2T_CONVERTER x2t-converter)

# Path to source files
set(X2T_DIR ${CORE_DIR}/X2tConverter)
message(STATUS "X2t converter source path: ${X2T_DIR}")

# Library source .h .cpp
file(GLOB X2T_CPP
    ${X2T_DIR}/src/cextracttools.cpp
    ${X2T_DIR}/src/ASCConverters.cpp
    ${X2T_DIR}/../Common/OfficeFileFormatChecker2.cpp
)

# Set targer as static library
add_library(${LIB_NAME_X2T_CONVERTER}
    STATIC
        ${X2T_CPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_X2T_CONVERTER}
    PUBLIC
        ${X2T_DIR}/src
    PRIVATE
        ${X2T_DIR}/../Common
)

# Set target definition
target_compile_definitions(${LIB_NAME_X2T_CONVERTER}
    PUBLIC
        _UNICODE
        UNICODE
    PRIVATE
        FILTER_FLATE_DECODE_ENABLED
        DONT_WRITE_EMBEDDED_FONTS
        AVS_USE_CONVERT_PPTX_TOCUSTOM_VML
)