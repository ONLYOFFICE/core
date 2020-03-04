cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_GRAPHICS graphics)

# Path to source files
set(GRAPHICS_EDITOR_DIR ${CORE_DIR}/DesktopEditor/graphics)
message(STATUS "Graphics editor source path: ${GRAPHICS_EDITOR_DIR}")

set(RASTER_EDITOR_DIR ${CORE_DIR}/DesktopEditor/raster)
message(STATUS "Raster editor source path: ${RASTER_EDITOR_DIR}")

# Graphics library source .h .cpp
file(GLOB GRAPHICS_CPP
    ${GRAPHICS_EDITOR_DIR}/*.cpp
    ${GRAPHICS_EDITOR_DIR}/pro/*.cpp
)

# Exclude sources
list(REMOVE_ITEM GRAPHICS_CPP
    ${GRAPHICS_EDITOR_DIR}/pro/lepton_lib_all.cpp
    ${GRAPHICS_EDITOR_DIR}/pro/graphics_pri.cpp
)

# Raster library source .h .cpp
file(GLOB RASTER_CPP
    ${RASTER_EDITOR_DIR}/BgraFrame.cpp
    ${RASTER_EDITOR_DIR}/ImageFileFormatChecker.cpp
)

file(GLOB RASTER_JBIG2_CPP ${RASTER_EDITOR_DIR}/JBig2/source/*.cpp)
file(GLOB RASTER_JBIG2_ENCODER_CPP ${RASTER_EDITOR_DIR}/JBig2/source/Encoder/*.cpp)
file(GLOB RASTER_JBIG2_LEPTON_CPP ${RASTER_EDITOR_DIR}/JBig2/source/LeptonLib/*.cpp)
file(GLOB RASTER_JP2_CPP ${RASTER_EDITOR_DIR}/Jp2/*.cpp)
file(GLOB RASTER_METAFILE_CPP ${RASTER_EDITOR_DIR}/Metafile/MetaFile.cpp)
file(GLOB RASTER_METAFILE_COMMON_CPP ${RASTER_EDITOR_DIR}/Metafile/Common/*.cpp)
file(GLOB RASTER_METAFILE_EMF_CPP ${RASTER_EDITOR_DIR}/Metafile/Emf/*.cpp)
file(GLOB RASTER_METAFILE_STARVIEW_CPP ${RASTER_EDITOR_DIR}/Metafile/StarView/*.cpp)
file(GLOB RASTER_METAFILE_WMF_CPP ${RASTER_EDITOR_DIR}/Metafile/Wmf/*.cpp)
file(GLOB RASTER_METAFILE_SVG_CPP ${RASTER_EDITOR_DIR}/Metafile/svg/*.cpp)


# Set targer as static library
add_library(${LIB_NAME_GRAPHICS}
    STATIC
        ${GRAPHICS_CPP}
        ${RASTER_CPP}
        ${RASTER_JBIG2_CPP}
        ${RASTER_JBIG2_ENCODER_CPP}
        ${RASTER_JBIG2_LEPTON_CPP}
        ${RASTER_JP2_CPP}
        ${RASTER_METAFILE_CPP}
        ${RASTER_METAFILE_COMMON_CPP}
        ${RASTER_METAFILE_EMF_CPP}
        ${RASTER_METAFILE_STARVIEW_CPP}
        ${RASTER_METAFILE_WMF_CPP}
        ${RASTER_METAFILE_SVG_CPP}
        ${WORKAROUND_DIR}/pthread_setcanceltype/pthread_setcanceltype.c
)

# Add dependency library
target_link_libraries(${LIB_NAME_GRAPHICS}
    PUBLIC
        freetype
        agg
)

# Add include files .h
target_include_directories(${LIB_NAME_GRAPHICS}
    PUBLIC
        ${GRAPHICS_EDITOR_DIR}
        ${RASTER_EDITOR_DIR}
    PRIVATE
        ${DESKTOP_EDITOR_DIR}
        ${GRAPHICS_EDITOR_DIR}/pro
        ${WORKAROUND_DIR}/pthread_setcanceltype
)