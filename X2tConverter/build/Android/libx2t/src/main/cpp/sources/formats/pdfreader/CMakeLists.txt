cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_PDF_READER pdf-reader)

# Path to source files
set(PDF_READER_DIR ${CORE_DIR}/PdfReader)
message(STATUS "Pdf reader source path: ${PDF_READER_DIR}")

# Library source .h .cpp
file(GLOB PDF_READER_CPP
    ${PDF_READER_DIR}/PdfReader.cpp
    ${PDF_READER_DIR}/Src/*.cpp
)

# Set targer as static library
add_library(${LIB_NAME_PDF_READER}
    STATIC
        ${PDF_READER_CPP}
)