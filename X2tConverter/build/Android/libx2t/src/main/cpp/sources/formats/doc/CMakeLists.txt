cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_DOC doc)

# Path to source files
set(DOC_FILE_DIR ${CORE_DIR}/ASCOfficeDocFile)
message(STATUS "Doc file source path: ${DOC_FILE_DIR}")

# Library source .h .cpp
file(GLOB DOC_CPP
    ${DOC_FILE_DIR}/DocFormatLib/DocFormatLib.cpp
    ${DOC_FILE_DIR}/DocFormatLib/Linux/docformatlib_converter.cpp
    ${DOC_FILE_DIR}/DocDocxConverter/EncryptionHeader.cpp
    ${DOC_FILE_DIR}/DocDocxConverter/DrawingPrimitives.cpp
    ${DOC_FILE_DIR}/DocDocxConverter/Spa.cpp
    ${DOC_FILE_DIR}/DocDocxConverter/OleObject.cpp
)

# Set targer as static library
add_library(${LIB_NAME_DOC}
    STATIC
        ${DOC_CPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_DOC}
    PRIVATE
        ${DOC_FILE_DIR}/DocFormatLib/Linux
)

# Set target definition
target_compile_definitions(${LIB_NAME_DOC}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
)