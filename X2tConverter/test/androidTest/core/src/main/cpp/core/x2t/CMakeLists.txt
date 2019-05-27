cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED X2T_DIR)
    message(FATAL_ERROR "You must set path in \"X2T_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_X2T)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_X2T\"!")
endif()

# Library source .h .cpp
file(GLOB X2T_CPP
    ${X2T_DIR}src/cextracttools.cpp
    ${X2T_DIR}src/ASCConverters.cpp
    ${X2T_DIR}../Common/OfficeFileFormatChecker2.cpp
)

# Set targer as static library
add_library(${LIB_NAME_X2T} STATIC ${X2T_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_X2T}
    PUBLIC
        ${LIB_NAME_FONT_ENGINE}
)

# Add include files .h
target_include_directories(${LIB_NAME_X2T}
    PUBLIC
        ${X2T_DIR}src/
        ${X2T_DIR}../Common/
)

# Set target definition
target_compile_definitions(${LIB_NAME_X2T}
    PUBLIC
        _UNICODE
        UNICODE
        FILTER_FLATE_DECODE_ENABLED
        DONT_WRITE_EMBEDDED_FONTS
        AVS_USE_CONVERT_PPTX_TOCUSTOM_VML
)

# Set target definition
target_compile_options(${LIB_NAME_X2T}
    PRIVATE
        -Wno-register
)