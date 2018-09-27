cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED RTF_FILE_DIR)
    message(FATAL_ERROR "You must set path in \"RTF_FILE_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_RTF)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_RTF\"!")
endif()

# Library source .h .cpp
file(GLOB RTF_CPP
    ${RTF_FILE_DIR}RtfFormatLib/Linux/RtfFormatLib.cpp
    ${RTF_FILE_DIR}RtfFormatLib/Linux/rtfformatlib_source.cpp
    ${RTF_FILE_DIR}RtfFormatLib/source/DestinationCommand.cpp
    ${RTF_FILE_DIR}RtfFormatLib/source/ConvertationManager.cpp
)

# Set targer as static library
add_library(${LIB_NAME_RTF} STATIC ${RTF_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_RTF}
    PUBLIC
        ${LIB_NAME_FONT_ENGINE}
)

# Add include files .h
target_include_directories(${LIB_NAME_RTF}
    PUBLIC
        ${RTF_FILE_DIR}RtfFormatLib/Linux/
        ${RTF_FILE_DIR}source/
        ${RTF_FILE_DIR}source/Reader/
        ${RTF_FILE_DIR}source/Writer/
)

# Set target definition
target_compile_definitions(${LIB_NAME_RTF}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
        AVS_USE_CONVERT_PPTX_TOCUSTOM_VML
)
