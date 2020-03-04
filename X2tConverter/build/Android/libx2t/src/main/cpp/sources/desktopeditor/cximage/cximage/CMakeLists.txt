cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

if (NOT DEFINED WORKAROUND_DIR)
    message(FATAL_ERROR "You must set path in \"WORKAROUND_DIR\" to \"workaround\" source!")
endif()

# Name for library
set(LIB_NAME_CXIMAGE cximage)

# Path to source files
set(CXIMAGE_DIR ${CORE_DIR}/DesktopEditor/cximage/CxImage)
message(STATUS "CxImage source path: ${CXIMAGE_DIR}")

# Delete source file .h .cpp
#file(REMOVE ${CXIMAGE_DIR}/stdint.h)

file(GLOB CXIMAGE_CPP
    ${CXIMAGE_DIR}/*.cpp
    ${WORKAROUND_DIR}/swab/*.c
)

# Add dependency project
add_subdirectory(../jpeg x2t/desktopeditor/cximage/jpeg)
add_subdirectory(../png x2t/desktopeditor/cximage/png)
add_subdirectory(../tiff x2t/desktopeditor/cximage/tiff)
add_subdirectory(../mng x2t/desktopeditor/cximage/mng)
add_subdirectory(../raw x2t/desktopeditor/cximage/raw)
add_subdirectory(../libpsd x2t/desktopeditor/cximage/libpsd)
add_subdirectory(../zlib x2t/desktopeditor/cximage/zlib)
add_subdirectory(../jasper x2t/desktopeditor/cximage/jasper)

# Set targer as static library
add_library(${LIB_NAME_CXIMAGE} STATIC ${CXIMAGE_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_CXIMAGE}
    PRIVATE
        jpeg
        png
        jasper
        tiff
        mng
        raw
        psd
        zlib
)

# Add include files .h
target_include_directories(${LIB_NAME_CXIMAGE}
    PRIVATE
        ${WORKAROUND_DIR}/swab/
)


# Set target definition
target_compile_definitions(${LIB_NAME_CXIMAGE}
    PRIVATE
        _LINUX
)