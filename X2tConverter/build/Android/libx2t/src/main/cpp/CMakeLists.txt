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
if (NOT DEFINED ARG_PATH_LIB_BUILD_TOOLS)
    message(FATAL_ERROR "You must set argument \"ARG_PATH_LIB_BUILD_TOOLS\" with path to 3d-party library...")
elseif (NOT EXISTS ${ARG_PATH_LIB_BUILD_TOOLS})
    file(MAKE_DIRECTORY ${ARG_PATH_LIB_BUILD_TOOLS})
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

# X2tConverter library name
if (NOT DEFINED ARG_NAME_LIB)
    message(FATAL_ERROR "You must set argument \"ARG_NAME_LIB\" with x2tConverter lib name...")
endif()

# ---------- Names ----------
# X2tConverter lib
set(LIB_NAME_X2T_CONVERTER ${ARG_NAME_LIB})

# ---------- Paths sources  ----------
# Core src dir path
set(CORE_DIR ${ARG_PATH_SRC_CORE})
set(X2T_CONVERTER_DEST ${ARG_PATH_LIB_DST}/${ANDROID_ABI})
message(STATUS "X2t Converter destination path: ${X2T_CONVERTER_DEST}")

# Prebuild libraries path
set(X2T_CONVERTER_LIBS ${ARG_PATH_LIB_BUILD_TOOLS}/${ANDROID_ABI})
message(STATUS "Prebuild libraries path: ${X2T_CONVERTER_LIBS}")

FILE(GLOB new_list "${X2T_CONVERTER_LIBS}/*.so")
SET(libs_list "")
FOREACH(file_path ${new_list})
    GET_FILENAME_COMPONENT(file_path ${file_path} NAME)
    SET(libs_list ${libs_list} ${file_path})
ENDFOREACH()
message(AUTHOR_WARNING "[libs_list] ${libs_list}")

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
)

# Link global libraries
link_libraries(
    android
    log
)

# Add target library
add_library(${LIB_NAME_X2T_CONVERTER}
    SHARED
        ${CORE_DIR}/DesktopEditor/fontengine/FontsAssistant.cpp
        ${CORE_DIR}/DesktopEditor/fontengine/ApplicationFontsWorker.cpp
        jni/X2t.cpp
)

FOREACH(lib ${libs_list})
    add_library(${lib} SHARED IMPORTED)
    set_target_properties(${lib} PROPERTIES IMPORTED_LOCATION
            ${X2T_CONVERTER_LIBS}/${lib})
ENDFOREACH()


# Export lib to common libs path
set_target_properties(${LIB_NAME_X2T_CONVERTER}
    PROPERTIES LIBRARY_OUTPUT_DIRECTORY
        ${X2T_CONVERTER_DEST}
)
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

# Add dependency library
target_link_libraries(${LIB_NAME_X2T_CONVERTER}
        # Converter
        ${libs_list}
)

# Add include files .h
target_include_directories(${LIB_NAME_X2T_CONVERTER}
    PUBLIC
        jni/objects
        jni/utils
)