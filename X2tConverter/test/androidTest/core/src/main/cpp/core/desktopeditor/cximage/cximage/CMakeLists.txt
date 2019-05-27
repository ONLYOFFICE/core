cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CXIMAGE_DIR)
    message(FATAL_ERROR "You must set path in \"CXIMAGE_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_CXIMAGE)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_CXIMAGE\"!")
endif()

# Delete source file .h .cpp
file(REMOVE ${CXIMAGE_DIR}stdint.h)

file(GLOB CXIMAGE_CPP
    ${CXIMAGE_DIR}*.cpp
    ${WORKAROUND_DIR}/swab/*.c
)

# Add dependency project
add_subdirectory(${SRC_DIR}/core/desktopeditor/cximage/jpeg ${BUILD_DIR}/core/desktopeditor/cximage/jpeg)
add_subdirectory(${SRC_DIR}/core/desktopeditor/cximage/png ${BUILD_DIR}/core/desktopeditor/cximage/png)
add_subdirectory(${SRC_DIR}/core/desktopeditor/cximage/tiff ${BUILD_DIR}/core/desktopeditor/cximage/tiff)
add_subdirectory(${SRC_DIR}/core/desktopeditor/cximage/mng ${BUILD_DIR}/core/desktopeditor/cximage/mng)
add_subdirectory(${SRC_DIR}/core/desktopeditor/cximage/raw ${BUILD_DIR}/core/desktopeditor/cximage/raw)
add_subdirectory(${SRC_DIR}/core/desktopeditor/cximage/libpsd ${BUILD_DIR}/core/desktopeditor/cximage/libpsd)
add_subdirectory(${SRC_DIR}/core/desktopeditor/cximage/zlib ${BUILD_DIR}/core/desktopeditor/cximage/zlib)
add_subdirectory(${SRC_DIR}/core/desktopeditor/cximage/jasper ${BUILD_DIR}/core/desktopeditor/cximage/jasper)

# Set targer as static library
add_library(${LIB_NAME_CXIMAGE} STATIC ${CXIMAGE_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_CXIMAGE}
    PUBLIC
        ${LIB_NAME_JPEG}
        ${LIB_NAME_PNG}
        ${LIB_NAME_JASPER}
        ${LIB_NAME_TIFF}
        ${LIB_NAME_MNG}
        ${LIB_NAME_RAW}
        ${LIB_NAME_PSD}
        ${LIB_NAME_ZLIB}
)

# Add include files .h
target_include_directories(${LIB_NAME_CXIMAGE}
    PUBLIC
        ${CXIMAGE_DIR}
        ${WORKAROUND_DIR}/swab/
)

# Add compile options
target_compile_options(${LIB_NAME_CXIMAGE}
    PUBLIC
        -Wno-register
)

# Set target definition
target_compile_definitions(${LIB_NAME_CXIMAGE}
    PRIVATE
        _LINUX
)