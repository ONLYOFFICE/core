cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

if (NOT DEFINED WORKAROUND_DIR)
    message(FATAL_ERROR "You must set path in \"WORKAROUND_DIR\" to \"workaround\" source!")
endif()

# Name for library
set(LIB_NAME_DOCXFORMAT docxformat)

# Path to source files
set(DOCXFORMAT_DIR ${CORE_DIR}/Common/DocxFormat)
message(STATUS "Docx format source path: ${DOCXFORMAT_DIR}")

# Library source .h .cpp
file(GLOB DOCXFORMAT_CPP
    ${WORKAROUND_DIR}/gcvt/*.c
#    ${DOCXFORMAT_DIR}/DocxFormatLib/common_formatlib.cpp
    ${DOCXFORMAT_DIR}/DocxFormatLib/docxformatlib.cpp
    ${DOCXFORMAT_DIR}/DocxFormatLib/docxformatlib_logic.cpp
    ${DOCXFORMAT_DIR}/DocxFormatLib/xlsxformatlib_logic.cpp
    ${DOCXFORMAT_DIR}/Source/Utility/codecvt.cpp
    ${DOCXFORMAT_DIR}/Source/Utility/DateTime.cpp
    ${DOCXFORMAT_DIR}/Source/Utility/TxtFile.cpp
    ${DOCXFORMAT_DIR}/Source/Base/unicode_util.cpp
)

# Set targer as static library
add_library(${LIB_NAME_DOCXFORMAT}
    STATIC
        ${DOCXFORMAT_CPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_DOCXFORMAT}
    PRIVATE
        ${DOCXFORMAT_DIR}/DocxFormatLib/
        ${DOCXFORMAT_DIR}/Source/
        ${WORKAROUND_DIR}/gcvt/
)

# Set target definition
target_compile_definitions(${LIB_NAME_DOCXFORMAT}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
)
