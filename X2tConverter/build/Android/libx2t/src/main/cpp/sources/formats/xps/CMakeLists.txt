cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_XPS xps)

# Path to source files
set(XPS_FILE_DIR ${CORE_DIR}/XpsFile)
message(STATUS "Xps file source path: ${XPS_FILE_DIR}")

# Library source .h .cpp
file(GLOB XPS_CPP
        ${XPS_FILE_DIR}/XpsFile.cpp
        ${XPS_FILE_DIR}/XpsLib/ContextState.cpp
        ${XPS_FILE_DIR}/XpsLib/Document.cpp
        ${XPS_FILE_DIR}/XpsLib/Page.cpp
        ${XPS_FILE_DIR}/XpsLib/StaticResources.cpp
        ${XPS_FILE_DIR}/XpsLib/Utils.cpp
        ${XPS_FILE_DIR}/XpsLib/WString.cpp
)

# Set targer as static library
add_library(${LIB_NAME_XPS}
    STATIC
        ${XPS_CPP}
)

# Set target definition
target_compile_definitions(${LIB_NAME_XPS}
    PRIVATE
        XPS_USE_DYNAMIC_LIBRARY
)