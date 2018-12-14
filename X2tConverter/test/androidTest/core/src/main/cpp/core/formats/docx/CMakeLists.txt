cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED DOCX_FILE_DIR)
    message(FATAL_ERROR "You must set path in \"DOCX_FILE_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_DOCX)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_DOCX\"!")
endif()

# Library source .h .cpp
file(GLOB DOCX_CPP
    ${DOCX_FILE_DIR}DocWrapper/DocxSerializer.cpp
    ${DOCX_FILE_DIR}DocWrapper/FontProcessor.cpp
    ${DOCX_FILE_DIR}DocWrapper/XlsxSerializer.cpp
    ${DOCX_FILE_DIR}DocWrapper/ChartWriter.cpp
    ${DOCX_FILE_DIR}BinWriter/BinWriters.cpp
    ${DOCX_FILE_DIR}BinReader/Readers.cpp
    ${DOCX_FILE_DIR}BinReader/CustormXmlWriter.cpp
)

# Set targer as static library
add_library(${LIB_NAME_DOCX} STATIC ${DOCX_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_DOCX}
    PUBLIC
        ${LIB_NAME_FONT_ENGINE}
)

# Add include files .h
target_include_directories(${LIB_NAME_DOCX}
    PUBLIC
        ${DOCX_FILE_DIR}DocWrapper/
        ${DOCX_FILE_DIR}BinReader/
        ${DOCX_FILE_DIR}BinWriter/
)

# Set target definition
target_compile_definitions(${LIB_NAME_DOCX}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
        SOLUTION_ASCOFFICEDOCXFILE2
)