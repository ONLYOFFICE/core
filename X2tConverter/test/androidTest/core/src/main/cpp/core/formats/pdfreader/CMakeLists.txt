cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED PDF_READER_DIR)
    message(FATAL_ERROR "You must set path in \"PDF_READER_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_PDF_READER)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_PDF_READER\"!")
endif()

# Library source .h .cpp
file(GLOB PDF_READER_CPP
    ${PDF_READER_DIR}PdfReader.cpp
    ${PDF_READER_DIR}Src/*.cpp
)

# Set targer as static library
add_library(${LIB_NAME_PDF_READER} STATIC ${PDF_READER_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_PDF_READER}
    PUBLIC
        ${LIB_NAME_FONT_ENGINE}
)

# Add include files .h
target_include_directories(${LIB_NAME_PDF_READER}
    PUBLIC
        ${PDF_READER_DIR}
    PRIVATE
        ${PDF_READER_DIR}Src/
        ${PDF_READER_DIR}Resources/
)

# Set target definition
target_compile_options(${LIB_NAME_PDF_READER}
    PRIVATE
        -Wno-c++11-narrowing
        -Wno-format-security
        -Wno-register
)