cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_CRYPTO_READER crypto-reader)

# Path to source files
set(CRYPTO_READER_DIR ${CORE_DIR}/OfficeCryptReader)
message(STATUS "Crypto reader source path: ${CRYPTO_READER_DIR}")

# Library source .h .cpp
file(GLOB CRYPTO_READER_CPP
    ${CRYPTO_READER_DIR}/source/*.cpp
)

# Set targer as static library
add_library(${LIB_NAME_CRYPTO_READER}
    STATIC
        ${CRYPTO_READER_CPP}
)