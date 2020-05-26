cmake_minimum_required(VERSION 3.4.1)

##### Information #####
#
# 1) Example path to build.ninja for debug arch armeabi-v7a:
#       ./libx2t/.externalNativeBuild/cmake/debug/armeabi-v7a/build.ninja
#
# 2) Example path to temp files like .o or .a:
#       ./libx2t/.externalNativeBuild/cmake/debug/armeabi-v7a/tmp/..
#
# 3) Crash with all optimisation flags on (SIGBUS (signal SIGBUS: illegal alignment)), line 410:
#       ../core/DesktopEditor/fontengine/ApplicationFonts.cpp

# Examples
# Exclude from list by regex
#list(FILTER XML_EDITOR_CPP EXCLUDE REGEX "^${XML_EDITOR_DIR}libxml2/test.*\.c$")

set(CMAKE_VERBOSE_MAKEFILE on)


macro(set_release_oflags flag)
    string(REGEX REPLACE "([ ]*-O.)" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REGEX REPLACE "([ ]*-O.)" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${flag}")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${flag}")
endmacro()

macro(set_debug_oflags flag)
    string(REGEX REPLACE "([ ]*-O.)" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    string(REGEX REPLACE "([ ]*-O.)" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${flag}")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${flag}")
endmacro()

macro(clear_oflags)
    string(REGEX REPLACE "([ ]*-O.)" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REGEX REPLACE "([ ]*-O.)" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endmacro()


# ---------- Arguments ----------
# 3dParty libs path arg
if (NOT DEFINED ARG_PATH_LIB_3D_PARTY)
    message(FATAL_ERROR "You must set argument \"ARG_PATH_LIB_3D_PARTY\" with path to 3d-party library...")
elseif (NOT EXISTS ${ARG_PATH_LIB_3D_PARTY})
    file(MAKE_DIRECTORY ${ARG_PATH_LIB_3D_PARTY})
    message(STATUS "Destination 3dParty path doesn't exist, created!")
endif()

# X2t destination path
if (NOT DEFINED ARG_PATH_LIB_DST)
    message(FATAL_ERROR "You must set argument \"ARG_PATH_LIB_DST\" with path to x2t.so destination...")
elseif (NOT EXISTS ${ARG_PATH_LIB_DST})
    file(MAKE_DIRECTORY ${ARG_PATH_LIB_DST})
    message(STATUS "Destination library path doesn't exist, created!")
endif()

# Core source path
if (NOT DEFINED ARG_PATH_SRC_CORE)
    message(FATAL_ERROR "You must set argument \"ARG_PATH_SRC_CORE\" with path to core sources...")
endif()

# X2t library name
if (NOT DEFINED ARG_NAME_LIB)
    message(FATAL_ERROR "You must set argument \"ARG_NAME_LIB\" with x2t lib name...")
endif()


# ---------- Names ----------
# x2t lib
set(LIB_NAME_X2T ${ARG_NAME_LIB})


# ---------- Paths sources  ----------
# Core src dir path
set(CORE_DIR ${ARG_PATH_SRC_CORE})
set(X2T_DEST ${ARG_PATH_LIB_DST}/${ANDROID_ABI})
message(STATUS "X2t destination path: ${X2T_DEST}")

# Path to source files
set(X2T_DIR ${CORE_DIR}/X2tConverter)
message(STATUS "X2t converter source path: ${X2T_DIR}")


# Prebuild libraries path
set(LIBS_DIR ${ARG_PATH_LIB_3D_PARTY})
message(STATUS "Prebuild libraries path: ${LIBS_DIR}")
set(X2T_LIBS ${LIBS_DIR}/${ANDROID_ABI})
message(STATUS "Prebuild libraries path: ${X2T_LIBS}")
# configure import libs
set(X2T_INCLUDE ${CORE_DIR}/X2tConverter/src/dylib
        ${CORE_DIR}/UnicodeConverter
        ${CORE_DIR}/Common)

# DesktopEditor
set(DESKTOP_EDITOR_DIR ${CORE_DIR}/DesktopEditor)
message(STATUS "Desktop editor source path: ${DESKTOP_EDITOR_DIR}")
# Font engine
set(FONT_ENGINE_DIR ${DESKTOP_EDITOR_DIR}/fontengine)
message(STATUS "Font engine source path: ${FONT_ENGINE_DIR}")

# Iconv libs path
#[[set(ICONV_INCLUDE_DIR ${LIBS_DIR}/${LIB_NAME_ICONV}/${ANDROID_ABI}/include)
message(STATUS "Iconv include path: ${ICONV_INCLUDE_DIR}")

if (NOT EXISTS ${ICONV_INCLUDE_DIR})
    message(FATAL_ERROR "Iconv path with .h files doesn't exist!")
endif()

set(ICONV_LIB_DIR ${LIBS_DIR}/${LIB_NAME_ICONV}/${ANDROID_ABI}/lib)
message(STATUS "Iconv lib path: ${ICONV_LIB_DIR}")

if (NOT EXISTS ${ICONV_LIB_DIR})
    message(FATAL_ERROR "Iconv path with .a files doesn't exist!")
endif()]]


# Boost src dir path
#[[set(BOOST_INCLUDE_DIR ${CORE_DIR}/Common/3dParty/${LIB_NAME_BOOST}/boost_1_58_0)
message(STATUS "Boost include path: ${BOOST_INCLUDE_DIR}")

if (NOT EXISTS ${BOOST_INCLUDE_DIR})
    message(FATAL_ERROR "Boost path with .h files doesn't exist!")
endif()

set(BOOST_LIB_DIR ${LIBS_DIR}/${LIB_NAME_BOOST}/${ANDROID_ABI}/lib)
message(STATUS "Boost lib path: ${BOOST_LIB_DIR}")

if (NOT EXISTS ${BOOST_LIB_DIR})
    message(FATAL_ERROR "Boost path with .a files doesn't exist!")
endif()]]


# Icu src dir path
#[[set(ICU_INCLUDE_DIR ${LIBS_DIR}/${LIB_NAME_ICU}/${ANDROID_ABI}/include)
message(STATUS "Icu include path: ${ICU_INCLUDE_DIR}")

if (NOT EXISTS ${ICU_INCLUDE_DIR})
    message(FATAL_ERROR "Icu path with .h files doesn't exist!")
endif()

set(ICU_LIB_DIR ${LIBS_DIR}/${LIB_NAME_ICU}/${ANDROID_ABI}/lib)
message(STATUS "Icu lib path: ${ICU_LIB_DIR}")

if (NOT EXISTS ${ICU_LIB_DIR})
    message(FATAL_ERROR "Icu path with .h files doesn't exist!")
endif()]]


# Add global path for lib resources here
# Workaround src dir path
#[[set(WORKAROUND_DIR ${CMAKE_CURRENT_SOURCE_DIR}/workaround)
message(STATUS "Workaround source path: ${WORKAROUND_DIR}")
if (NOT EXISTS ${WORKAROUND_DIR})
    message(FATAL_ERROR "Workaround source path doesn't exist!")
endif()]]


# Show default compile flags
message(STATUS "Flags default for CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "Flags default for CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "Flags default for CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "Flags default for CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "Flags default for CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "Flags default for CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")

# Clear optimisation flags
clear_oflags()
# Set flags for all release targets
set_debug_oflags(-O0)
set_release_oflags(-O0)

# Set flags only for CPP source compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fexceptions -frtti")

# After flags cleared
message(STATUS "Flags cleared for CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "Flags cleared for CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")

# Globals parameters
# Set global definition
add_definitions(
    -DLINUX
    -D_LINUX
    -D__linux__
    -D__ANDROID__
    -D_ARM_ALIGN_
)

# Global compile flags
add_compile_options(
    -Wno-c++11-narrowing
    -Wno-format-security
    -Wno-register

# Flags for checks from https://developer.android.com/distribute/best-practices/develop/64-bit#port-32-to-64
    -Waddress-of-packed-member
    -Werror=pointer-to-int-cast
#    -Werror=int-to-pointer-cast
#    -Werror=implicit-function-declaration
#    -Werror=shorten-64-to-32
)

# 3d party libraries
# Dir with headers
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
   # ${ICONV_INCLUDE_DIR}
   # ${ICU_INCLUDE_DIR}
   # ${BOOST_INCLUDE_DIR}
)

#[[# Dir with libraries
link_directories(
    ${ICONV_LIB_DIR}
    ${ICU_LIB_DIR}
    ${BOOST_LIB_DIR}
)
]]
# Link global libraries
link_libraries(
    android
    log
   #[[ ${LIB_NAME_ICU_UC}
    ${LIB_NAME_ICU_i18n}
    ${LIB_NAME_ICU_DATA}
    ${LIB_NAME_ICONV}
    ${LIB_NAME_BOOST_REGEX}]]
)


# Add dependencies sub projects
#[[
add_subdirectory(sources/cpufeature x2t/cpufeature)

add_subdirectory(sources/common/officeutils x2t/common/officeutils)
add_subdirectory(sources/common/unicodeconverter x2t/common/unicodeconverter)
add_subdirectory(sources/common/cryptopp x2t/common/cryptopp)
add_subdirectory(sources/common/pole x2t/common/pole)
add_subdirectory(sources/common/downloader x2t/common/downloader)
add_subdirectory(sources/common/cryptoreader x2t/common/cryptoreader)
add_subdirectory(sources/common/xlsxserialize x2t/common/xlsxserialize)

add_subdirectory(sources/desktopeditor/freetypes x2t/desktopeditor/freetypes)
add_subdirectory(sources/desktopeditor/agg x2t/desktopeditor/agg)
add_subdirectory(sources/desktopeditor/common x2t/desktopeditor/common)
add_subdirectory(sources/desktopeditor/cximage/cximage x2t/desktopeditor/cximage/cximage)
add_subdirectory(sources/desktopeditor/graphics x2t/desktopeditor/graphics)
add_subdirectory(sources/desktopeditor/fontengine x2t/desktopeditor/fontengine)
add_subdirectory(sources/desktopeditor/xml x2t/desktopeditor/xml)
add_subdirectory(sources/desktopeditor/doctrender x2t/desktopeditor/doctrender)

add_subdirectory(sources/formats/pdfwriter x2t/formats/pdfwriter)
add_subdirectory(sources/formats/pdfreader x2t/formats/pdfreader)
add_subdirectory(sources/formats/djvu x2t/formats/djvu)
add_subdirectory(sources/formats/docxformat x2t/formats/docxformat)
add_subdirectory(sources/formats/htmlfile x2t/formats/htmlfile)
add_subdirectory(sources/formats/htmlrender x2t/formats/htmlrender)
add_subdirectory(sources/formats/xls x2t/formats/xls)
add_subdirectory(sources/formats/ppt x2t/formats/ppt)
add_subdirectory(sources/formats/pptx x2t/formats/pptx)
add_subdirectory(sources/formats/docx x2t/formats/docx)
add_subdirectory(sources/formats/doc x2t/formats/doc)
add_subdirectory(sources/formats/odfreader x2t/formats/odfreader)
add_subdirectory(sources/formats/odfwriter x2t/formats/odfwriter)
add_subdirectory(sources/formats/rtf x2t/formats/rtf)
add_subdirectory(sources/formats/txt x2t/formats/txt)
add_subdirectory(sources/formats/xps x2t/formats/xps)

add_subdirectory(sources/x2t x2t/x2t_converter)
]]

# Add target library
add_library(${LIB_NAME_X2T}
    SHARED
        ${FONT_ENGINE_DIR}/FontsAssistant.cpp
        ${FONT_ENGINE_DIR}/ApplicationFontsWorker.cpp
        jni/X2t.cpp
)
add_library(DjVuFile SHARED IMPORTED)
add_library(doctrenderer SHARED IMPORTED)
add_library(graphics SHARED IMPORTED)
add_library(HtmlFile SHARED IMPORTED)
add_library(HtmlRenderer SHARED IMPORTED)
add_library(kernel SHARED IMPORTED)
add_library(PdfReader SHARED IMPORTED)
add_library(PdfWriter SHARED IMPORTED)
add_library(UnicodeConverter SHARED IMPORTED)
add_library(x2t SHARED IMPORTED)
add_library(XpsFile SHARED IMPORTED)

# Export lib to common libs path
set_target_properties(${LIB_NAME_X2T}
    PROPERTIES LIBRARY_OUTPUT_DIRECTORY
        ${X2T_DEST}
)
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
set_target_properties(DjVuFile PROPERTIES IMPORTED_LOCATION
        ${X2T_LIBS}/libDjVuFile.so)
set_target_properties(doctrenderer PROPERTIES IMPORTED_LOCATION
        ${X2T_LIBS}/libdoctrenderer.so)
set_target_properties(graphics PROPERTIES IMPORTED_LOCATION
        ${X2T_LIBS}/libgraphics.so)
set_target_properties(HtmlFile PROPERTIES IMPORTED_LOCATION
        ${X2T_LIBS}/libHtmlFile.so)
set_target_properties(HtmlRenderer PROPERTIES IMPORTED_LOCATION
        ${X2T_LIBS}/libHtmlRenderer.so)
set_target_properties(kernel PROPERTIES IMPORTED_LOCATION
        ${X2T_LIBS}/libkernel.so)
set_target_properties(PdfReader PROPERTIES IMPORTED_LOCATION
        ${X2T_LIBS}/libPdfReader.so)
set_target_properties(PdfWriter PROPERTIES IMPORTED_LOCATION
        ${X2T_LIBS}/libPdfWriter.so)
set_target_properties(UnicodeConverter PROPERTIES IMPORTED_LOCATION
        ${X2T_LIBS}/libUnicodeConverter.so)
set_target_properties(x2t PROPERTIES IMPORTED_LOCATION
        ${X2T_LIBS}/libx2t.so)
set_target_properties(XpsFile PROPERTIES IMPORTED_LOCATION
        ${X2T_LIBS}/libXpsFile.so)

# Add dependency library
target_link_libraries(${LIB_NAME_X2T}

        # Converter
        DjVuFile
        doctrenderer
        graphics
        HtmlFile
        HtmlRenderer
        kernel
        PdfReader
        PdfWriter
        UnicodeConverter
        x2t
        XpsFile
)

# Add include files .h
target_include_directories(${LIB_NAME_X2T}
    PUBLIC
        jni/objects
        jni/utils
        ${X2T_INCLUDE}
    PRIVATE
        ${CORE_DIR}
)

#include(ndk-stl-config.cmake)