cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_PPT ppt)

# Path to source files
set(PPT_FILE_DIR ${CORE_DIR}/ASCOfficePPTFile)
message(STATUS "Ppt file source path: ${PPT_FILE_DIR}")

# Library source .h .cpp
file(GLOB PTT_CPP
    ${PPT_FILE_DIR}/PPTFormatLib/Linux/pptformatlib_logic.cpp
    ${PPT_FILE_DIR}/PPTFormatLib/PPTFormatLib.cpp
)

# Set targer as static library
add_library(${LIB_NAME_PPT}
    STATIC
        ${PTT_CPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_PPT}
    PRIVATE
        ${PPT_FILE_DIR}/PPTFormatLib
        ${PPT_FILE_DIR}/PPTFormatLib/Linux
        ${PPT_FILE_DIR}/PPTFormatLib/Reader
        ${PPT_FILE_DIR}/PPTFormatLib/Records
        ${PPT_FILE_DIR}/PPTFormatLib/PPTXWriter
)

# Set target definition
target_compile_definitions(${LIB_NAME_PPT}
    PRIVATE
        _UNICODE
        _PRESENTATION_WRITER_
        _SVG_CONVERT_TO_IMAGE_
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
)
