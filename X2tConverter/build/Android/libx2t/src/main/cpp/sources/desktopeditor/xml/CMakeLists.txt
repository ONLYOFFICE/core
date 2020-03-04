cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_EDITOR_XML editor-xml)

# Path to source files
set(XML_EDITOR_DIR ${CORE_DIR}/DesktopEditor/xml)
message(STATUS "Xml editor source path: ${XML_EDITOR_DIR}")

# Library source .h .cpp
file(GLOB XML_EDITOR_CPP
    ${XML_EDITOR_DIR}/build/qt/libxml2_all.c
    ${XML_EDITOR_DIR}/build/qt/libxml2_all2.c
    ${XML_EDITOR_DIR}/src/*.cpp
)

# Set targer as static library
add_library(${LIB_NAME_EDITOR_XML}
    STATIC
        ${XML_EDITOR_CPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_EDITOR_XML}
    PUBLIC
        ${XML_EDITOR_DIR}/include/
    PRIVATE
        ${XML_EDITOR_DIR}/build/qt/
        ${XML_EDITOR_DIR}/libxml2/
        ${XML_EDITOR_DIR}/libxml2/include/
        ${XML_EDITOR_DIR}/src/
)

# Set target definition
target_compile_definitions(${LIB_NAME_EDITOR_XML}
    PRIVATE
        LIBXML_READER_ENABLED
        LIBXML_READER_ENABLED
        LIBXML_PUSH_ENABLED
        LIBXML_HTML_ENABLED
        LIBXML_XPATH_ENABLED
        LIBXML_OUTPUT_ENABLED
        LIBXML_C14N_ENABLED
        LIBXML_SAX1_ENABLED
        LIBXML_TREE_ENABLED
        LIBXML_XPTR_ENABLED
        IN_LIBXML
        LIBXML_STATIC
        HAVE_VA_COPY
)