cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED FONT_ENGINE_DIR)
    message(FATAL_ERROR "You must set path in \"FONT_ENGINE_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_FONT_ENGINE)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_FONT_ENGINE\"!")
endif()

# Lib src path
if (NOT DEFINED GRAPHICS_EDITOR_DIR)
    message(FATAL_ERROR "You must set path in \"GRAPHICS_EDITOR_DIR\"!")
endif()

# Lib src path
if (NOT DEFINED RASTER_EDITOR_DIR)
    message(FATAL_ERROR "You must set path in \"RASTER_EDITOR_DIR\"!")
endif()

# Engine library source .h .cpp
file(GLOB FONT_ENGINE_CPP
    ${FONT_ENGINE_DIR}*.cpp
    ${FONT_ENGINE_DIR}fontconverter/*.cpp
)

# Graphics library source .h .cpp
file(GLOB GRAPHICS_CPP
    ${GRAPHICS_EDITOR_DIR}*.cpp
    ${GRAPHICS_EDITOR_DIR}pro/*.cpp
)

# Exclude sources
list(REMOVE_ITEM GRAPHICS_CPP
    ${GRAPHICS_EDITOR_DIR}pro/lepton_lib_all.cpp
    ${GRAPHICS_EDITOR_DIR}pro/graphics_pri.cpp
)

# Raster library source .h .cpp
file(GLOB RASTER_CPP
    ${RASTER_EDITOR_DIR}BgraFrame.cpp
    ${RASTER_EDITOR_DIR}ImageFileFormatChecker.cpp
)

file(GLOB RASTER_JBIG2_CPP ${RASTER_EDITOR_DIR}JBig2/source/*.cpp)
file(GLOB RASTER_JBIG2_ENCODER_CPP ${RASTER_EDITOR_DIR}JBig2/source/Encoder/*.cpp)
file(GLOB RASTER_JBIG2_LEPTON_CPP ${RASTER_EDITOR_DIR}JBig2/source/LeptonLib/*.cpp)
file(GLOB RASTER_JP2_CPP ${RASTER_EDITOR_DIR}Jp2/*.cpp)
file(GLOB RASTER_METAFILE_CPP ${RASTER_EDITOR_DIR}Metafile/MetaFile.cpp)
file(GLOB RASTER_METAFILE_COMMON_CPP ${RASTER_EDITOR_DIR}Metafile/Common/*.cpp)
file(GLOB RASTER_METAFILE_EMF_CPP ${RASTER_EDITOR_DIR}Metafile/Emf/*.cpp)
file(GLOB RASTER_METAFILE_STARVIEW_CPP ${RASTER_EDITOR_DIR}Metafile/StarView/*.cpp)
file(GLOB RASTER_METAFILE_WMF_CPP ${RASTER_EDITOR_DIR}Metafile/Wmf/*.cpp)

## Concatenate with raster resources
set(FONT_ENGINE_CPP ${FONT_ENGINE_CPP}
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
)

# Set targer as static library
add_library(${LIB_NAME_FONT_ENGINE}
    STATIC
        ${FONT_ENGINE_CPP}
        ${WORKAROUND_DIR}/pthread_setcanceltype/pthread_setcanceltype.c
)

# Add dependency library
target_link_libraries(${LIB_NAME_FONT_ENGINE}
    PUBLIC
        ${LIB_NAME_FREETYPES}
        ${LIB_NAME_AGG}
        ${LIB_NAME_CXIMAGE}
        ${LIB_NAME_EDITOR_COMMON}
)

# Add include files .h
target_include_directories(${LIB_NAME_FONT_ENGINE}
    PUBLIC
        ${FONT_ENGINE_DIR}
        ${FONT_ENGINE_DIR}fontconverter
        ${GRAPHICS_EDITOR_DIR}
        ${GRAPHICS_EDITOR_DIR}pro
        ${WORKAROUND_DIR}/pthread_setcanceltype
)

# Add compile options
target_compile_options(${LIB_NAME_FONT_ENGINE}
    PUBLIC
        -Wno-register
        -Wno-c++11-narrowing
)

# Set target definition
target_compile_definitions(${LIB_NAME_FONT_ENGINE}
    PRIVATE
        _LINUX
)