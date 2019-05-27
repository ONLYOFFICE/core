cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED DOCT_RENDER_DIR)
    message(FATAL_ERROR "You must set path in \"DOCT_RENDER_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_DOCT_RENDER)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_DOCT_RENDER\"!")
endif()

# Library source .h .cpp
file(GLOB DOCT_RENDER_CPP
    #${DOCT_RENDER_DIR}memorystream.cpp
    #${DOCT_RENDER_DIR}nativecontrol.cpp
    #${DOCT_RENDER_DIR}doctrenderer.cpp
    #${DOCT_RENDER_DIR}docbuilder.cpp
    ${DOCT_RENDER_DIR}doctrenderer_android.cpp
)

# Set targer as static library
add_library(${LIB_NAME_DOCT_RENDER} STATIC ${DOCT_RENDER_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_DOCT_RENDER}
    PUBLIC
        ${LIB_NAME_FONT_ENGINE}
)

# Add include files .h
target_include_directories(${LIB_NAME_DOCT_RENDER}
    PRIVATE
        ${DOCT_RENDER_DIR}
)

# Set target definition
target_compile_definitions(${LIB_NAME_DOCT_RENDER}
    PRIVATE
        BUIDLER_OPEN_DOWNLOAD_ENABLED
        BUIDLER_OPEN_BASE64_ENABLED
)

# Set target definition
target_compile_options(${LIB_NAME_DOCT_RENDER}
    PRIVATE
        -Wno-format-security
)