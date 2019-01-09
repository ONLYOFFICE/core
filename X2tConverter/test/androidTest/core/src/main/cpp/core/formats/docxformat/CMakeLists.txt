cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED DOCXFORMAT_DIR)
    message(FATAL_ERROR "You must set path in \"DOCXFORMAT_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_DOCXFORMAT)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_DOCXFORMAT\"!")
endif()

# Library source .h .cpp
file(GLOB DOCXFORMAT_CPP
    ${WORKAROUND_DIR}/gcvt/*.c
    ${DOCXFORMAT_DIR}DocxFormatLib/docxformatlib.cpp
    ${DOCXFORMAT_DIR}DocxFormatLib/docxformatlib_logic.cpp
    ${DOCXFORMAT_DIR}Source/Utility/codecvt.cpp
    ${DOCXFORMAT_DIR}Source/Utility/DateTime.cpp
    ${DOCXFORMAT_DIR}Source/Utility/TxtFile.cpp
    ${DOCXFORMAT_DIR}Source/Base/unicode_util.cpp
)

# Set targer as static library
add_library(${LIB_NAME_DOCXFORMAT} STATIC ${DOCXFORMAT_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_DOCXFORMAT}
    PUBLIC
        ${LIB_NAME_FONT_ENGINE}
)

# Add include files .h
target_include_directories(${LIB_NAME_DOCXFORMAT}
    PRIVATE
        ${DOCXFORMAT_DIR}DocxFormatLib/
        ${WORKAROUND_DIR}/gcvt/
        ${DOCXFORMAT_DIR}Source/
        ${DOCXFORMAT_DIR}Source/Base/
        ${DOCXFORMAT_DIR}Source/Utility/
        ${DOCXFORMAT_DIR}Source/DocxFormat/
        ${DOCXFORMAT_DIR}Source/DocxFormat/Diagram/
        ${DOCXFORMAT_DIR}Source/DocxFormat/Drawing/
        ${DOCXFORMAT_DIR}Source/DocxFormat/External/
        ${DOCXFORMAT_DIR}Source/DocxFormat/Logic/
        ${DOCXFORMAT_DIR}Source/DocxFormat/Math/
        ${DOCXFORMAT_DIR}Source/DocxFormat/Media/
        ${DOCXFORMAT_DIR}Source/DocxFormat/Settings/
        ${DOCXFORMAT_DIR}Source/XlsxFormat/
        ${DOCXFORMAT_DIR}Source/XlsxFormat/CalcChain/
        ${DOCXFORMAT_DIR}Source/XlsxFormat/Chart/
        ${DOCXFORMAT_DIR}Source/XlsxFormat/Comments/
        ${DOCXFORMAT_DIR}Source/XlsxFormat/Controls/
        ${DOCXFORMAT_DIR}Source/XlsxFormat/Drawing/
        ${DOCXFORMAT_DIR}Source/XlsxFormat/ExternalLinks/
        ${DOCXFORMAT_DIR}Source/XlsxFormat/Ole/
        ${DOCXFORMAT_DIR}Source/XlsxFormat/Pivot/
        ${DOCXFORMAT_DIR}Source/XlsxFormat/SharedStrings/
        ${DOCXFORMAT_DIR}Source/XlsxFormat/Styles/
        ${DOCXFORMAT_DIR}Source/XlsxFormat/Table/
        ${DOCXFORMAT_DIR}Source/XlsxFormat/Workbook/
        ${DOCXFORMAT_DIR}Source/XlsxFormat/Worksheets/
)

# Set target definition
target_compile_definitions(${LIB_NAME_DOCXFORMAT}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
)
