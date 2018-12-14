cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED HTML_FILE_DIR)
    message(FATAL_ERROR "You must set path in \"HTML_FILE_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_HTML_FILE)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_HTML_FILE\"!")
endif()

# Library source .h .cpp
file(GLOB HTML_FILE_CPP
    ${HTML_FILE_DIR}*.cpp
)

# Set targer as static library
add_library(${LIB_NAME_HTML_FILE} STATIC ${HTML_FILE_CPP})

# Add include files .h
target_include_directories(${LIB_NAME_HTML_FILE}
    PUBLIC
        ${HTML_FILE_DIR}
)

# Set target definition
target_compile_definitions(${LIB_NAME_HTML_FILE}
    PRIVATE
        LINUX
        HTMLFILE_USE_DYNAMIC_LIBRARY
        UNICODECONVERTER_USE_DYNAMIC_LIBRARY
        asc_static_link_libstd
)

# Set target definition
target_compile_options(${LIB_NAME_HTML_FILE}
    PRIVATE
        -Wno-format-security
)