cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_PPTX pptx)

# Path to source files
set(PPTX_FILE_DIR ${CORE_DIR}/ASCOfficePPTXFile)
message(STATUS "Pptx file source path: ${PPTX_FILE_DIR}")

# Library source .h .cpp
file(GLOB PTTX_CPP
    ${PPTX_FILE_DIR}/ASCOfficeDrawingConverter.cpp
    ${PPTX_FILE_DIR}/ASCOfficePPTXFileRealization.cpp
    ${PPTX_FILE_DIR}/PPTXLib/Linux/PPTXFormatLib/pptxformatlib.cpp
    ${PPTX_FILE_DIR}/PPTXLib/Linux/PPTXFormatLib/pptxformatlib_logic.cpp
    ${PPTX_FILE_DIR}/Editor/Drawing/Shapes/BaseShape/BaseShape.cpp
    ${PPTX_FILE_DIR}/Editor/Drawing/Shapes/BaseShape/toVmlConvert.cpp
    ${PPTX_FILE_DIR}/Editor/Drawing/Shapes/BaseShape/PPTShape/PptFormula.cpp
    ${PPTX_FILE_DIR}/Editor/Drawing/Shapes/BaseShape/PPTShape/PptShape.cpp
    ${PPTX_FILE_DIR}/Editor/Drawing/Shapes/BaseShape/PPTXShape/PptxShape.cpp
    ${PPTX_FILE_DIR}/Editor/Drawing/Shapes/BaseShape/PPTXShape/PptxFormula.cpp
    ${PPTX_FILE_DIR}/Editor/BinaryFileReaderWriter.cpp
    ${PPTX_FILE_DIR}/Editor/FontPicker.cpp
    ${PPTX_FILE_DIR}/Editor/Drawing/TextAttributesEx.cpp
    ${PPTX_FILE_DIR}/Editor/Drawing/Elements.cpp
)

# Set targer as static library
add_library(${LIB_NAME_PPTX}
    STATIC
        ${PTTX_CPP}
)

# Set target definition
target_compile_definitions(${LIB_NAME_PPTX}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
        AVS_USE_CONVERT_PPTX_TOCUSTOM_VML
)