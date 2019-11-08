cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_DOCT_RENDER doct-render)

# Path to source files
set(DOCT_RENDER_DIR ${CORE_DIR}/DesktopEditor/doctrenderer)
message(STATUS "Doct draw source path: ${DOCT_RENDER_DIR}")

# Library source .h .cpp
file(GLOB DOCT_RENDER_CPP
#    ${DOCT_RENDER_DIR}/memorystream.cpp
#    ${DOCT_RENDER_DIR}/nativecontrol.cpp
    ${DOCT_RENDER_DIR}/doctrenderer_empty.cpp
#    ${DOCT_RENDER_DIR}/docbuilder.cpp
)

# Set targer as static library
add_library(${LIB_NAME_DOCT_RENDER}
    STATIC
        ${DOCT_RENDER_CPP}
)

# Add dependency library
target_link_libraries(${LIB_NAME_DOCT_RENDER}
    PUBLIC
        font-engine
)

# Set target definition
target_compile_definitions(${LIB_NAME_DOCT_RENDER}
    PRIVATE
        BUIDLER_OPEN_DOWNLOAD_ENABLED
        BUIDLER_OPEN_BASE64_ENABLED
)