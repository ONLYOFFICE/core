cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED DOWNLOADER_DIR)
    message(FATAL_ERROR "You must set path in \"DOWNLOADER_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_DOWNLOADER)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_DOWNLOADER\"!")
endif()

# Library source .h .cpp
file(GLOB DOWNLOADER_CPP
        ${DOWNLOADER_DIR}FileDownloader.cpp
        ${DOWNLOADER_DIR}FileDownloader_curl.cpp
)

# Set targer as static library
add_library(${LIB_NAME_DOWNLOADER} STATIC ${DOWNLOADER_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_DOWNLOADER}
    PRIVATE
        ${LIB_NAME_FONT_ENGINE}
        ${LIB_NAME_CURL}
        ${LIB_NAME_SSL}
        ${LIB_NAME_CRYPTO}
)

# Add include files .h
target_include_directories(${LIB_NAME_DOWNLOADER}
    PRIVATE
        ${DOWNLOADER_DIR}
)