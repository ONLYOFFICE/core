cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED DOC_FILE_DIR)
    message(FATAL_ERROR "You must set path in \"DOC_FILE_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_DOC)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_DOC\"!")
endif()

# Library source .h .cpp
file(GLOB DOC_CPP
    ${DOC_FILE_DIR}DocFormatLib/DocFormatLib.cpp
    ${DOC_FILE_DIR}DocFormatLib/Linux/docformatlib_converter.cpp
    ${DOC_FILE_DIR}DocDocxConverter/EncryptionHeader.cpp
    ${DOC_FILE_DIR}DocDocxConverter/DrawingPrimitives.cpp
    ${DOC_FILE_DIR}DocDocxConverter/Spa.cpp
    ${DOC_FILE_DIR}DocDocxConverter/OleObject.cpp
)

# Set targer as static library
add_library(${LIB_NAME_DOC} STATIC ${DOC_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_DOC}
    PUBLIC
        ${LIB_NAME_FONT_ENGINE}
)

# Add include files .h
target_include_directories(${LIB_NAME_DOC}
    PRIVATE
        ${DOC_FILE_DIR}DocFormatLib/
        ${DOC_FILE_DIR}DocFormatLib/Linux/
        ${DOC_FILE_DIR}Common/
        ${DOC_FILE_DIR}DocDocxConverter/
        ${DOC_FILE_DIR}DocDocxConverter/OfficeDrawing/
        ${DOC_FILE_DIR}DocDocxConverter/OfficeDrawing/Shapetypes/
)

# Set target definition
target_compile_definitions(${LIB_NAME_DOC}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
)