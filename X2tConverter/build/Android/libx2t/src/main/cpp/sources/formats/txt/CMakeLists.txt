cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_TXT txt)

# Path to source files
set(TXT_DIR ${CORE_DIR}/ASCOfficeTxtFile)
message(STATUS "Txt file source path: ${TXT_DIR}")

# Library source .h .cpp
file(GLOB TXT_CPP
    ${TXT_DIR}/TxtXmlFormatLib/Source/Common/Encoding.cpp
    ${TXT_DIR}/TxtXmlFormatLib/Source/Common/ToString.cpp
    ${TXT_DIR}/TxtXmlFormatLib/Source/TxtFormat/File.cpp
    ${TXT_DIR}/TxtXmlFormatLib/Source/TxtFormat/TxtFile.cpp
    ${TXT_DIR}/TxtXmlFormatLib/Source/TxtXmlFile.cpp
    ${TXT_DIR}/TxtXmlFormatLib/Source/ConvertDocx2Txt.cpp
    ${TXT_DIR}/TxtXmlFormatLib/Source/ConvertTxt2Docx.cpp
)

# Set targer as static library
add_library(${LIB_NAME_TXT}
    STATIC
        ${TXT_CPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_TXT}
    PRIVATE
        ${TXT_DIR}/TxtXmlFormatLib/Source
        ${TXT_DIR}/TxtXmlFormatLib/Source/Common
        ${TXT_DIR}/TxtXmlFormatLib/Source/TxtFormat
)

# Set target definition
target_compile_definitions(${LIB_NAME_TXT}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
)