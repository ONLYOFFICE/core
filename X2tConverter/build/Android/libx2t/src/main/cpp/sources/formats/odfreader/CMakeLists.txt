cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_ODF_READER odf-reader)

# Path to source files
set(ODF_READER_FILE_DIR ${CORE_DIR}/ASCOfficeOdfFile)
message(STATUS "Odf reader file source path: ${ODF_READER_FILE_DIR}")

# Library source .h .cpp
file(GLOB ODF_CPP
    ${ODF_READER_FILE_DIR}/linux/odffilereaderlib_odf.cpp
    ${ODF_READER_FILE_DIR}/linux/odffilereaderlib_odf_datatypes.cpp
    ${ODF_READER_FILE_DIR}/linux/odffilereaderlib_oox.cpp
    ${ODF_READER_FILE_DIR}/formulasconvert/formulasconvert_oox.cpp
    ${ODF_READER_FILE_DIR}/formulasconvert/formulasconvert_odf.cpp
    ${ODF_READER_FILE_DIR}/src/conversionelement.cpp
    ${ODF_READER_FILE_DIR}/src/xml/attributes.cpp
    ${ODF_READER_FILE_DIR}/src/xml/sax.cpp
    ${ODF_READER_FILE_DIR}/src/xml/sax_xmllite.cpp
    ${ODF_READER_FILE_DIR}/src/xml/utils.cpp
    ${ODF_READER_FILE_DIR}/src/xml/xmlchar.cpp
    ${ODF_READER_FILE_DIR}/src/common/CPColorUtils.cpp
    ${ODF_READER_FILE_DIR}/src/common/CPString.cpp
    ${ODF_READER_FILE_DIR}/src/common/readdocelement.cpp
    ${ODF_READER_FILE_DIR}/src/ConvertOO2OOX.cpp
)

# Set targer as static library
add_library(${LIB_NAME_ODF_READER}
    STATIC
        ${ODF_CPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_ODF_READER}
    PUBLIC
        ${ODF_READER_FILE_DIR}/include
        ${ODF_READER_FILE_DIR}/src/odf/datatypes
    PRIVATE
        ${ODF_READER_FILE_DIR}/linux
        ${ODF_READER_FILE_DIR}/formulasconvert
        ${ODF_READER_FILE_DIR}/src
        ${ODF_READER_FILE_DIR}/src/xml
        ${ODF_READER_FILE_DIR}/src/common
)

# Set target definition
target_compile_definitions(${LIB_NAME_ODF_READER}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
)
