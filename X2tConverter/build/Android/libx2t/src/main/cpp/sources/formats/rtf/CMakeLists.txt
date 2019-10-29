cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CORE_DIR)
    message(FATAL_ERROR "You must set path in \"CORE_DIR\" to \"core\" source!")
endif()

# Name for library
set(LIB_NAME_RTF rtf)

# Path to source files
set(RTF_FILE_DIR ${CORE_DIR}/ASCOfficeRtfFile)
message(STATUS "Rtf file source path: ${RTF_FILE_DIR}")

# Library source .h .cpp
file(GLOB RTF_CPP
    ${RTF_FILE_DIR}/RtfFormatLib/Linux/RtfFormatLib.cpp
    ${RTF_FILE_DIR}/RtfFormatLib/Linux/rtfformatlib_source.cpp
    ${RTF_FILE_DIR}/RtfFormatLib/source/DestinationCommand.cpp
    ${RTF_FILE_DIR}/RtfFormatLib/source/ConvertationManager.cpp
)

# Set targer as static library
add_library(${LIB_NAME_RTF}
    STATIC
        ${RTF_CPP}
)

# Set target definition
target_compile_definitions(${LIB_NAME_RTF}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
        AVS_USE_CONVERT_PPTX_TOCUSTOM_VML
)
