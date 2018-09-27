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

# Library source .h .cpp
file(GLOB OFFICE_UTILS_CPP
        ${OFFICE_UTILS_DIR}src/*.cpp
        ${OFFICE_UTILS_DIR}src/zlib-1.2.3/*.c
        ${OFFICE_UTILS_DIR}src/zlib-1.2.3/contrib/minizip/*.c
)

# Exclude sources
list(REMOVE_ITEM OFFICE_UTILS_CPP
    ${OFFICE_UTILS_DIR}src/zlib-1.2.3/contrib/minizip/iowin32.c
)

# Set targer as static library
add_library(${LIB_NAME_OFFICE_UTILS} STATIC ${OFFICE_UTILS_CPP})

# Add dependency library
#target_link_libraries(${LIB_NAME_OFFICE_UTILS}
#    PRIVATE
#        ${LIB_NAME_EDITOR_COMMON}
#)

# Add include files .h
target_include_directories(${LIB_NAME_OFFICE_UTILS}
    PUBLIC
        ${OFFICE_UTILS_DIR}
        ${OFFICE_UTILS_DIR}zlib-1.2.3/
    PRIVATE
        ${OFFICE_UTILS_DIR}zlib-1.2.3/contrib/minizip/
)

# Set target definition
target_compile_definitions(${LIB_NAME_OFFICE_UTILS}
    PRIVATE
        _LINUX
        BUILD_ZLIB_AS_SOURCES
)