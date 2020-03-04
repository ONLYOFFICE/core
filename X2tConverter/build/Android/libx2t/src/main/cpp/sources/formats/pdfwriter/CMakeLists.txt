cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_PDF_WRITER pdf-writer)

# Path to source files
set(PDF_WRITER_DIR ${CORE_DIR}/PdfWriter)
message(STATUS "Pdf writer source path: ${PDF_WRITER_DIR}")

# Library source .h .cpp
file(GLOB PDF_WRITER_CPP
    ${PDF_WRITER_DIR}/*.cpp
    ${PDF_WRITER_DIR}/Src/*.cpp
)

# Set targer as static library
add_library(${LIB_NAME_PDF_WRITER}
    STATIC
        ${PDF_WRITER_CPP}
)

# Add dependency library
target_link_libraries(${LIB_NAME_PDF_WRITER}
    PRIVATE
        freetype
)

# Set target definition
target_compile_definitions(${LIB_NAME_PDF_WRITER}
    PRIVATE
        PDFWRITER_USE_DYNAMIC_LIBRARY
)