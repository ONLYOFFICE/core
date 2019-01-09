cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED ODF_READER_FILE_DIR)
    message(FATAL_ERROR "You must set path in \"ODF_READER_FILE_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_ODF_READER)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_ODF_READER\"!")
endif()

# Library source .h .cpp
file(GLOB ODF_CPP
    ${ODF_READER_FILE_DIR}linux/odffilereaderlib_odf.cpp
    ${ODF_READER_FILE_DIR}linux/odffilereaderlib_odf_datatypes.cpp
    ${ODF_READER_FILE_DIR}linux/odffilereaderlib_oox.cpp
    ${ODF_READER_FILE_DIR}formulasconvert/formulasconvert_oox.cpp
    ${ODF_READER_FILE_DIR}formulasconvert/formulasconvert_odf.cpp
    ${ODF_READER_FILE_DIR}src/conversionelement.cpp
    ${ODF_READER_FILE_DIR}src/xml/attributes.cpp
    ${ODF_READER_FILE_DIR}src/xml/sax.cpp
    ${ODF_READER_FILE_DIR}src/xml/sax_xmllite.cpp
    ${ODF_READER_FILE_DIR}src/xml/utils.cpp
    ${ODF_READER_FILE_DIR}src/xml/xmlchar.cpp
    ${ODF_READER_FILE_DIR}src/common/CPColorUtils.cpp
    ${ODF_READER_FILE_DIR}src/common/CPString.cpp
    ${ODF_READER_FILE_DIR}src/common/readdocelement.cpp
    ${ODF_READER_FILE_DIR}src/ConvertOO2OOX.cpp
)

# Set targer as static library
add_library(${LIB_NAME_ODF_READER} STATIC ${ODF_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_ODF_READER}
    PRIVATE
        ${LIB_NAME_DOCXFORMAT}
        ${LIB_NAME_EDITOR_XML}
)

# Add include files .h
target_include_directories(${LIB_NAME_ODF_READER}
    PRIVATE
        ${ODF_READER_FILE_DIR}include/
        ${ODF_READER_FILE_DIR}linux/
        ${ODF_READER_FILE_DIR}formulasconvert/
        ${ODF_READER_FILE_DIR}src/
        ${ODF_READER_FILE_DIR}src/xml/
        ${ODF_READER_FILE_DIR}src/common/
)

# Set target definition
target_compile_definitions(${LIB_NAME_ODF_READER}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
)
