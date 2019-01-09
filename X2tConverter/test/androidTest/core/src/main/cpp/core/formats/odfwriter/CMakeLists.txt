cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED ODF_WRITER_FILE_DIR)
    message(FATAL_ERROR "You must set path in \"ODF_WRITER_FILE_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_ODF_WRITER)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_ODF_WRITER\"!")
endif()

# Library source .h .cpp
file(GLOB ODF_CPP
    ${ODF_WRITER_FILE_DIR}linux/odffilewriterlib_odf.cpp
    ${ODF_WRITER_FILE_DIR}source/Oox2OdfConverter/Converter.cpp
    ${ODF_WRITER_FILE_DIR}source/Oox2OdfConverter/ConverterChart.cpp
    ${ODF_WRITER_FILE_DIR}source/Oox2OdfConverter/ConvertVml.cpp
    ${ODF_WRITER_FILE_DIR}source/Oox2OdfConverter/DocxConverter.cpp
    ${ODF_WRITER_FILE_DIR}source/Oox2OdfConverter/XlsxConverter.cpp
    ${ODF_WRITER_FILE_DIR}source/Oox2OdfConverter/ConvertDrawing.cpp
    ${ODF_WRITER_FILE_DIR}source/Oox2OdfConverter/PptxConverter.cpp
)

# Set targer as static library
add_library(${LIB_NAME_ODF_WRITER} STATIC ${ODF_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_ODF_WRITER}
    PUBLIC
        ${LIB_NAME_FONT_ENGINE}
)

# Add dependency library
target_link_libraries(${LIB_NAME_ODF_WRITER}
    PRIVATE
        ${LIB_NAME_ODF_READER}
)

# Add include files .h
target_include_directories(${LIB_NAME_ODF_WRITER}
    PRIVATE
        ${ODF_WRITER_FILE_DIR}linux/
        ${ODF_WRITER_FILE_DIR}source/
        ${ODF_WRITER_FILE_DIR}source/OdfFormat/
        ${ODF_WRITER_FILE_DIR}source/OdfFormat/Shapes/
        ${ODF_WRITER_FILE_DIR}source/Oox2OdfConverter/
        ${ODF_READER_FILE_DIR}include/
        ${ODF_READER_FILE_DIR}linux/
        ${ODF_READER_FILE_DIR}formulasconvert/
        ${ODF_READER_FILE_DIR}src/
        ${ODF_READER_FILE_DIR}src/xml/
        ${ODF_READER_FILE_DIR}src/common/
        ${ODF_READER_FILE_DIR}src/odf/datatypes
)

# Set target definition
target_compile_definitions(${LIB_NAME_ODF_WRITER}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
)
