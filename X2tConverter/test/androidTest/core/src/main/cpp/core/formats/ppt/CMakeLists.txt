cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED PPT_FILE_DIR)
    message(FATAL_ERROR "You must set path in \"PPT_FILE_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_PPT)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_PPT\"!")
endif()

# Library source .h .cpp
file(GLOB PTT_CPP
    ${PPT_FILE_DIR}PPTFormatLib/Linux/pptformatlib_logic.cpp
    ${PPT_FILE_DIR}PPTFormatLib/PPTFormatLib.cpp
)

# Set targer as static library
add_library(${LIB_NAME_PPT} STATIC ${PTT_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_PPT}
    PRIVATE
        ${LIB_NAME_XLS}
        ${LIB_NAME_DOWNLOADER}
)

# Add include files .h
target_include_directories(${LIB_NAME_PPT}
    PUBLIC
        ${PPT_FILE_DIR}PPTFormatLib/
        ${PPT_FILE_DIR}PPTFormatLib/Linux/
        ${PPT_FILE_DIR}PPTFormatLib/Reader/
        ${PPT_FILE_DIR}PPTFormatLib/Records/
        ${PPT_FILE_DIR}PPTFormatLib/PPTXWriter/
)

# Set target definition
target_compile_definitions(${LIB_NAME_PPT}
    PRIVATE
        _UNICODE
        _PRESENTATION_WRITER_
        _SVG_CONVERT_TO_IMAGE_
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
)
