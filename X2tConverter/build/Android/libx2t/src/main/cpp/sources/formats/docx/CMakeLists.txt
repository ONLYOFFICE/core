cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_DOCX docx)

# Path to source files
set(DOCX_FILE_DIR ${CORE_DIR}/ASCOfficeDocxFile2)
message(STATUS "Docx file source path: ${DOCX_FILE_DIR}")

# Library source .h .cpp
file(GLOB DOCX_CPP
    ${DOCX_FILE_DIR}/DocWrapper/DocxSerializer.cpp
    ${DOCX_FILE_DIR}/DocWrapper/FontProcessor.cpp
    ${DOCX_FILE_DIR}/DocWrapper/XlsxSerializer.cpp
    ${DOCX_FILE_DIR}/DocWrapper/ChartWriter.cpp
    ${DOCX_FILE_DIR}/BinWriter/BinWriters.cpp
    ${DOCX_FILE_DIR}/BinReader/Readers.cpp
    ${DOCX_FILE_DIR}/BinReader/CustormXmlWriter.cpp
)

# Set targer as static library
add_library(${LIB_NAME_DOCX}
    STATIC
        ${DOCX_CPP}
)

# Set target definition
target_compile_definitions(${LIB_NAME_DOCX}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
        SOLUTION_ASCOFFICEDOCXFILE2
        AVS_USE_CONVERT_PPTX_TOCUSTOM_VML
)