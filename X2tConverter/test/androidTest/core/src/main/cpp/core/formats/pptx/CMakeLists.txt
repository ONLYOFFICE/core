cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED PPTX_FILE_DIR)
    message(FATAL_ERROR "You must set path in \"PPTX_FILE_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_PPTX)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_PPTX\"!")
endif()

# Library source .h .cpp
file(GLOB PTTX_CPP
    ${PPTX_FILE_DIR}ASCOfficeDrawingConverter.cpp
    ${PPTX_FILE_DIR}ASCOfficePPTXFileRealization.cpp
    ${PPTX_FILE_DIR}PPTXLib/Linux/PPTXFormatLib/pptxformatlib.cpp
    ${PPTX_FILE_DIR}PPTXLib/Linux/PPTXFormatLib/pptxformatlib_logic.cpp
    ${PPTX_FILE_DIR}Editor/Drawing/Shapes/BaseShape/BaseShape.cpp
    ${PPTX_FILE_DIR}Editor/Drawing/Shapes/BaseShape/PPTShape/PptFormula.cpp
    ${PPTX_FILE_DIR}Editor/Drawing/Shapes/BaseShape/PPTShape/PptShape.cpp
    ${PPTX_FILE_DIR}Editor/Drawing/Shapes/BaseShape/PPTXShape/PptxShape.cpp
    ${PPTX_FILE_DIR}Editor/Drawing/Shapes/BaseShape/PPTXShape/PptxFormula.cpp
    ${PPTX_FILE_DIR}Editor/BinaryFileReaderWriter.cpp
    ${PPTX_FILE_DIR}Editor/FontPicker.cpp
    ${PPTX_FILE_DIR}Editor/Drawing/TextAttributesEx.cpp
    ${PPTX_FILE_DIR}Editor/Drawing/Elements.cpp
)

# Set targer as static library
add_library(${LIB_NAME_PPTX} STATIC ${PTTX_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_PPTX}
    PRIVATE
        ${LIB_NAME_DOCXFORMAT}
        ${LIB_NAME_XLS}
        ${LIB_NAME_HTML_RENDER}
        ${LIB_NAME_XLSX_SERIALIZE}
)

# Add include files .h
target_include_directories(${LIB_NAME_PPTX}
    PUBLIC
        ${PPTX_FILE_DIR}
        ${PPTX_FILE_DIR}PPTXLib/Linux/PPTXFormatLib/
        ${PPTX_FILE_DIR}Editor/
        ${PPTX_FILE_DIR}Editor/Drawing/
        ${PPTX_FILE_DIR}Editor/Drawing/Shapes/
        ${PPTX_FILE_DIR}Editor/Drawing/Shapes/BaseShape/
        ${PPTX_FILE_DIR}Editor/Drawing/Shapes/BaseShape/PPTShape/
        ${PPTX_FILE_DIR}Editor/Drawing/Shapes/BaseShape/PPTShape/PPTAutoShapes/
        ${PPTX_FILE_DIR}Editor/Drawing/Shapes/BaseShape/PPTXShape/
        ${PPTX_FILE_DIR}Editor/Drawing/Shapes/BaseShape/PPTXShape/OOXMLShapes/
        ${PPTX_FILE_DIR}PPTXFormat/
        ${PPTX_FILE_DIR}PPTXFormat/Limit/
        ${PPTX_FILE_DIR}PPTXFormat/Presentation/
        ${PPTX_FILE_DIR}PPTXFormat/Theme/
        ${PPTX_FILE_DIR}PPTXFormat/ViewProps/
        ${PPTX_FILE_DIR}PPTXFormat/Logic/
        ${PPTX_FILE_DIR}PPTXFormat/Logic/Bullets/
        ${PPTX_FILE_DIR}PPTXFormat/Logic/Colors/
        ${PPTX_FILE_DIR}PPTXFormat/Logic/Effects/
        ${PPTX_FILE_DIR}PPTXFormat/Logic/Fills/
        ${PPTX_FILE_DIR}PPTXFormat/Logic/Media/
        ${PPTX_FILE_DIR}PPTXFormat/Logic/Path2D/
        ${PPTX_FILE_DIR}PPTXFormat/Logic/Runs/
        ${PPTX_FILE_DIR}PPTXFormat/Logic/Table/
        ${PPTX_FILE_DIR}PPTXFormat/Logic/Media/
        ${PPTX_FILE_DIR}PPTXFormat/Logic/Timing/
        ${PPTX_FILE_DIR}PPTXFormat/Logic/Transitions/
)

# Add compile options
target_compile_options(${LIB_NAME_PPTX}
    PUBLIC
        -Wno-format-security
)

# Set target definition
target_compile_definitions(${LIB_NAME_PPTX}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
        AVS_USE_CONVERT_PPTX_TOCUSTOM_VML
)