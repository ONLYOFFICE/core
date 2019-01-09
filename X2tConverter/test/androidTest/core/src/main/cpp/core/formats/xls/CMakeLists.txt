cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED XLS_FILE_DIR)
    message(FATAL_ERROR "You must set path in \"XLS_FILE_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_XLS)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_XLS\"!")
endif()

# Library source .h .cpp
file(GLOB XLS_CPP
    ${XLS_FILE_DIR}source/linux/xlsformatlib_converter.cpp
    ${XLS_FILE_DIR}source/linux/xlsformatlib_logic.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Auxiliary/HelpFunc.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Binary/CFRecord.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Binary/CFRecordType.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Binary/CFStream.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Binary/CFStreamCacheReader.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Binary/CFStreamCacheWriter.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Binary/CompoundFile.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Crypt/rtl/cipher.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Crypt/rtl/digest.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Crypt/BiffDecoder_RCF.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Crypt/BinaryCodec_RCF.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Crypt/Decryptor.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Crypt/RC4Crypt.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Crypt/XORCrypt.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Logging/Log.cpp
    ${XLS_FILE_DIR}source/XlsFormat/Logging/Logger.cpp
    ${XLS_FILE_DIR}source/Common/utils.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/ConvertShapes/FormulaShape.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/ConvertShapes/CustomShape.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/ConvertShapes/BaseShape_1.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/ConvertXls2Xlsx.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/external_items.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/mediaitems_utils.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/namespaces.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/oox_content_type.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/oox_package.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/oox_rels.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/XlsConverter.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_conversion_context.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_drawing_context.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_drawings.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_hyperlinks.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_output_xml.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_package.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_protection.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_tablecontext.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_textcontext.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_chart_context.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_pivots_context.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_sheet_context.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_external_context.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_activeX_context.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_comments.cpp
    ${XLS_FILE_DIR}source/XlsXlsxConverter/xlsx_comments_context.cpp
)

# Set targer as static library
add_library(${LIB_NAME_XLS} STATIC ${XLS_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_XLS}
    PUBLIC
        ${LIB_NAME_UNICODE_CONVERTER}
        ${LIB_NAME_ICONV}
        ${LIB_NAME_FONT_ENGINE}
)

# Add include files .h
target_include_directories(${LIB_NAME_XLS}
    PRIVATE
        ${XLS_FILE_DIR}source/Common/
        ${XLS_FILE_DIR}source/XlsFormat/
        ${XLS_FILE_DIR}source/XlsFormat/Auxiliary/
        ${XLS_FILE_DIR}source/XlsFormat/Binary/
        ${XLS_FILE_DIR}source/XlsFormat/Logging/
        ${XLS_FILE_DIR}source/XlsFormat/Crypt/
        ${XLS_FILE_DIR}source/XlsFormat/Crypt/rtl/
        ${XLS_FILE_DIR}source/XlsFormat/Logic/
        ${XLS_FILE_DIR}source/XlsFormat/Logic/Biff_records/
        ${XLS_FILE_DIR}source/XlsFormat/Logic/Biff_structures/
        ${XLS_FILE_DIR}source/XlsFormat/Logic/Biff_structures/ODRAW/
        ${XLS_FILE_DIR}source/XlsFormat/Logic/Biff_unions/
        ${XLS_FILE_DIR}source/XlsFormat/Logic/SummaryInformationStream/
        ${XLS_FILE_DIR}source/XlsFormat/Logic/SummaryInformationStream/Structures/
        ${XLS_FILE_DIR}source/XlsXlsxConverter/
        ${XLS_FILE_DIR}source/XlsXlsxConverter/ConvertShapes/
        ${XLS_FILE_DIR}source/XlsXlsxConverter/ConvertShapes/oldAutoShapes/
)

# Set target definition
target_compile_definitions(${LIB_NAME_XLS}
    PRIVATE
        #__linux__
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
)

# Set target definition
target_compile_options(${LIB_NAME_XLS}
    PRIVATE
        -Wno-register
)