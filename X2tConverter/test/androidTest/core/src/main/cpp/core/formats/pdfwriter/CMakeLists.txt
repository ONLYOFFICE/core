cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED PDF_WRITER_DIR)
    message(FATAL_ERROR "You must set path in \"PDF_WRITER_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_PDF_WRITER)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_PDF_WRITER\"!")
endif()

# Library source .h .cpp
file(GLOB PDF_WRITER_CPP
    ${PDF_WRITER_DIR}*.cpp
    ${PDF_WRITER_DIR}Src/*.cpp
)

# Set targer as static library
add_library(${LIB_NAME_PDF_WRITER} STATIC ${PDF_WRITER_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_PDF_WRITER}
    PRIVATE
        ${LIB_NAME_FREETYPES}
        ${LIB_NAME_FONT_ENGINE}
        ${LIB_NAME_UNICODE_CONVERTER}
)

# Add include files .h
target_include_directories(${LIB_NAME_PDF_WRITER}
    PUBLIC
        ${PDF_WRITER_DIR}
    PRIVATE
        ${PDF_WRITER_DIR}Src/
)

# Set target definition
target_compile_definitions(${LIB_NAME_PDF_WRITER}
    PRIVATE
        _LINUX
        PDFWRITER_USE_DYNAMIC_LIBRARY
)

# Set target definition
target_compile_options(${LIB_NAME_PDF_WRITER}
    PRIVATE
        -Wno-c++11-narrowing
        -Wno-format-security
        -Wno-register
)