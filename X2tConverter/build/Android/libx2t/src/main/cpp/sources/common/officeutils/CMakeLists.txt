cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_OFFICE_UTILS officeutils)

# Path to source files
set(OFFICE_UTILS_DIR ${CORE_DIR}/OfficeUtils)
message(STATUS "Office utils converter source path: ${OFFICE_UTILS_DIR}")

# Library source .h .cpp
file(GLOB OFFICE_UTILS_CPP
        ${OFFICE_UTILS_DIR}/src/*.cpp
        ${OFFICE_UTILS_DIR}/src/zlib*/*.c
        ${OFFICE_UTILS_DIR}/src/zlib*/contrib/minizip/*.c
)

# Exclude sources
list(FILTER OFFICE_UTILS_CPP
    EXCLUDE REGEX
        "^${OFFICE_UTILS_DIR}/src/zlib.*/contrib/minizip/iowin32.c$")

# Set targer as static library
add_library(${LIB_NAME_OFFICE_UTILS}
    STATIC
        ${OFFICE_UTILS_CPP}
)

# Set target definition
target_compile_definitions(${LIB_NAME_OFFICE_UTILS}
    PRIVATE
        _LINUX
        BUILD_ZLIB_AS_SOURCES
        USE_FILE32API
)