cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED XPS_FILE_DIR)
    message(FATAL_ERROR "You must set path in \"XPS_FILE_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_XPS)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_XPS\"!")
endif()

# Library source .h .cpp
file(GLOB XPS_CPP
    ${XPS_FILE_DIR}XpsFile.cpp
    ${XPS_FILE_DIR}XpsLib/ContextState.cpp
    ${XPS_FILE_DIR}XpsLib/Document.cpp
    ${XPS_FILE_DIR}XpsLib/Page.cpp
    ${XPS_FILE_DIR}XpsLib/StaticResources.cpp
    ${XPS_FILE_DIR}XpsLib/Utils.cpp
    ${XPS_FILE_DIR}XpsLib/WString.cpp
)

# Set targer as static library
add_library(${LIB_NAME_XPS} STATIC ${XPS_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_XPS}
    PUBLIC
        ${LIB_NAME_FONT_ENGINE}
)

# Add include files .h
target_include_directories(${LIB_NAME_XPS}
    PUBLIC
        ${XPS_FILE_DIR}
        ${XPS_FILE_DIR}XpsLib/
)

# Set target definition
target_compile_definitions(${LIB_NAME_XPS}
    PRIVATE
        XPS_USE_DYNAMIC_LIBRARY
)

# Set target definition
target_compile_options(${LIB_NAME_XPS}
    PRIVATE
        -Wno-register
)