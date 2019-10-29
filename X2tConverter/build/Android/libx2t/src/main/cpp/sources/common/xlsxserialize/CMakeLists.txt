cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_XLSX_SERIALIZE xlsx-serialize)

# Path to source files
set(XLSX_SERIALIZE_DIR ${CORE_DIR}/XlsxSerializerCom)
message(STATUS "Xlsx serialize source path: ${XLSX_SERIALIZE_DIR}")

# Library source .h .cpp
file(GLOB XLSX_SERIALIZE_CPP
    ${XLSX_SERIALIZE_DIR}/Common/*.cpp
    ${XLSX_SERIALIZE_DIR}/Reader/*.cpp
    ${XLSX_SERIALIZE_DIR}/Writer/*.cpp
)

# Set targer as static library
add_library(${LIB_NAME_XLSX_SERIALIZE}
    STATIC
        ${XLSX_SERIALIZE_CPP}
)

# Set target definition
target_compile_definitions(${LIB_NAME_XLSX_SERIALIZE}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
)