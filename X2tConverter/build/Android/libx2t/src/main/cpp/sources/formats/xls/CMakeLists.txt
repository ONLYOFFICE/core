cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_XLS xls)

# Path to source files
set(XLS_FILE_DIR ${CORE_DIR}/ASCOfficeXlsFile2)
message(STATUS "Xls file source path: ${XLS_FILE_DIR}")

# Library source .h .cpp
file(GLOB XLS_CPP
        ${XLS_FILE_DIR}/source/linux/xlsformatlib_converter.cpp
        ${XLS_FILE_DIR}/source/linux/xlsformatlib_logic.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Auxiliary/HelpFunc.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Binary/CFRecord.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Binary/CFRecordType.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Binary/CFStream.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Binary/CFStreamCacheReader.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Binary/CFStreamCacheWriter.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Binary/CompoundFile.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Crypt/rtl/cipher.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Crypt/rtl/digest.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Crypt/BiffDecoder_RCF.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Crypt/BinaryCodec_RCF.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Crypt/Decryptor.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Crypt/RC4Crypt.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Crypt/XORCrypt.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Logging/Log.cpp
        ${XLS_FILE_DIR}/source/XlsFormat/Logging/Logger.cpp
        ${XLS_FILE_DIR}/source/XlsXlsxConverter/xlsx_pivots_context.cpp
        ${XLS_FILE_DIR}/source/XlsXlsxConverter/xlsx_sheet_context.cpp
        ${XLS_FILE_DIR}/source/XlsXlsxConverter/xlsx_external_context.cpp
        ${XLS_FILE_DIR}/source/XlsXlsxConverter/xlsx_activeX_context.cpp
)

# Set targer as static library
add_library(${LIB_NAME_XLS}
    STATIC
        ${XLS_CPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_XLS}
    PRIVATE
        ${XLS_FILE_DIR}/source/Common
        ${XLS_FILE_DIR}/source/XlsFormat
)

# Set target definition
target_compile_definitions(${LIB_NAME_XLS}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
)