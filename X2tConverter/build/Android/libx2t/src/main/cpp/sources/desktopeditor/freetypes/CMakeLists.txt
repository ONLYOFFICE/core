cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_FREETYPES freetype)

# Path to source files
set(FREETYPES_DIR ${CORE_DIR}/DesktopEditor/freetype-2.5.2)
message(STATUS "FreeTypes source path: ${FREETYPES_DIR}")

# Library source .c
set(FREETYPES_CPP
    ${FREETYPES_DIR}/src/autofit/autofit.c
    ${FREETYPES_DIR}/src/base/ftadvanc.c
    ${FREETYPES_DIR}/src/base/ftbbox.c
    ${FREETYPES_DIR}/src/base/ftbitmap.c
    ${FREETYPES_DIR}/src/base/ftcalc.c
    ${FREETYPES_DIR}/src/base/ftcid.c
    ${FREETYPES_DIR}/src/base/ftdbgmem.c
    ${FREETYPES_DIR}/src/base/ftdebug.c
    ${FREETYPES_DIR}/src/base/ftfstype.c
    ${FREETYPES_DIR}/src/base/ftgasp.c
    ${FREETYPES_DIR}/src/base/ftgloadr.c
    ${FREETYPES_DIR}/src/base/ftglyph.c
    ${FREETYPES_DIR}/src/base/ftgxval.c
    ${FREETYPES_DIR}/src/base/ftinit.c
    ${FREETYPES_DIR}/src/base/ftlcdfil.c
    ${FREETYPES_DIR}/src/base/ftmm.c
    ${FREETYPES_DIR}/src/base/ftobjs.c
    ${FREETYPES_DIR}/src/base/ftotval.c
    ${FREETYPES_DIR}/src/base/ftoutln.c
    ${FREETYPES_DIR}/src/base/ftpatent.c
    ${FREETYPES_DIR}/src/base/ftpfr.c
    ${FREETYPES_DIR}/src/base/ftrfork.c
    ${FREETYPES_DIR}/src/base/ftsnames.c
    ${FREETYPES_DIR}/src/base/ftstream.c
    ${FREETYPES_DIR}/src/base/ftstroke.c
    ${FREETYPES_DIR}/src/base/ftsynth.c
    ${FREETYPES_DIR}/src/base/ftsystem.c
    ${FREETYPES_DIR}/src/base/fttrigon.c
    ${FREETYPES_DIR}/src/base/fttype1.c
    ${FREETYPES_DIR}/src/base/ftutil.c
    ${FREETYPES_DIR}/src/base/ftwinfnt.c
    ${FREETYPES_DIR}/src/base/ftxf86.c
    ${FREETYPES_DIR}/src/bdf/bdf.c
    ${FREETYPES_DIR}/src/bzip2/ftbzip2.c
    ${FREETYPES_DIR}/src/cache/ftcache.c
    ${FREETYPES_DIR}/src/cff/cff.c
    ${FREETYPES_DIR}/src/cid/type1cid.c
    ${FREETYPES_DIR}/src/gzip/ftgzip.c
    ${FREETYPES_DIR}/src/lzw/ftlzw.c
    ${FREETYPES_DIR}/src/pcf/pcf.c
    ${FREETYPES_DIR}/src/pfr/pfr.c
    ${FREETYPES_DIR}/src/psaux/psaux.c
    ${FREETYPES_DIR}/src/pshinter/pshinter.c
    ${FREETYPES_DIR}/src/psnames/psmodule.c
    ${FREETYPES_DIR}/src/raster/raster.c
    ${FREETYPES_DIR}/src/sfnt/sfnt.c
    ${FREETYPES_DIR}/src/smooth/smooth.c
    ${FREETYPES_DIR}/src/truetype/truetype.c
    ${FREETYPES_DIR}/src/type1/type1.c
    ${FREETYPES_DIR}/src/type42/type42.c
    ${FREETYPES_DIR}/src/winfonts/winfnt.c
)

# Set targer as static library
add_library(${LIB_NAME_FREETYPES}
    STATIC
        ${FREETYPES_CPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_FREETYPES}
    PUBLIC
        ${FREETYPES_DIR}/include/
    PRIVATE
        ${FREETYPES_DIR}/src/truetype/
        ${FREETYPES_DIR}/src/sfnt/
        ${FREETYPES_DIR}/src/autofit/
        ${FREETYPES_DIR}/src/smooth/
        ${FREETYPES_DIR}/src/raster/
        ${FREETYPES_DIR}/src/psaux/
        ${FREETYPES_DIR}/src/psnames/
)

# Set target definition
target_compile_definitions(${LIB_NAME_FREETYPES}
    PRIVATE
        FT2_BUILD_LIBRARY
)