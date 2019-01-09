cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED OFFICE_UTILS_DIR)
    message(FATAL_ERROR "You must set path in \"OFFICE_UTILS_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_OFFICE_UTILS)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_OFFICE_UTILS\"!")
endif()

# Zlib name
set(ZLIB_NAME zlib-1.2.11)

# Library source .h .cpp
file(GLOB OFFICE_UTILS_CPP
        ${OFFICE_UTILS_DIR}src/*.cpp
        ${OFFICE_UTILS_DIR}src/${ZLIB_NAME}/*.c
        ${OFFICE_UTILS_DIR}src/${ZLIB_NAME}/contrib/minizip/*.c
)

# Exclude sources
list(REMOVE_ITEM OFFICE_UTILS_CPP
    ${OFFICE_UTILS_DIR}src/${ZLIB_NAME}/contrib/minizip/iowin32.c
)

# Set targer as static library
add_library(${LIB_NAME_OFFICE_UTILS} STATIC ${OFFICE_UTILS_CPP})

# Add include files .h
target_include_directories(${LIB_NAME_OFFICE_UTILS}
    PRIVATE
        ${OFFICE_UTILS_DIR}
        ${OFFICE_UTILS_DIR}${ZLIB_NAME}/
        ${OFFICE_UTILS_DIR}${ZLIB_NAME}/contrib/minizip/
)

# Set target definition
target_compile_definitions(${LIB_NAME_OFFICE_UTILS}
    PRIVATE
        _LINUX
        USE_FILE32API
        NOCRYPT
        NOUNCRYPT
        BUILD_ZLIB_AS_SOURCES
)