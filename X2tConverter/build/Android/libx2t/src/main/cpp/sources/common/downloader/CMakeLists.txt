cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_DOWNLOADER downloader)

# Path to source files
set(DOWNLOADER_DIR ${CORE_DIR}/Common/FileDownloader)
message(STATUS "File downloader source path: ${DOWNLOADER_DIR}")

# Library source .h .cpp
file(GLOB DOWNLOADER_CPP
    ${DOWNLOADER_DIR}/FileDownloader.cpp
    ${DOWNLOADER_DIR}/FileDownloader_curl.cpp
)

# Set targer as static library
add_library(${LIB_NAME_DOWNLOADER}
    STATIC
        ${DOWNLOADER_CPP})

# Set target definition
target_compile_definitions(${LIB_NAME_DOWNLOADER}
    PRIVATE
        USE_EXTERNAL_DOWNLOAD
)