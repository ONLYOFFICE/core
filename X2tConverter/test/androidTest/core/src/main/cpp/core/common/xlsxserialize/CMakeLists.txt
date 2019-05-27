cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED XLSX_SERIALIZE_DIR)
    message(FATAL_ERROR "You must set path in \"XLSX_SERIALIZE_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_XLSX_SERIALIZE)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_XLSX_SERIALIZE\"!")
endif()

# Library source .h .cpp
file(GLOB XLSX_SERIALIZE_CPP
    ${XLSX_SERIALIZE_DIR}Common/*.cpp
    ${XLSX_SERIALIZE_DIR}Reader/*.cpp
    ${XLSX_SERIALIZE_DIR}Writer/*.cpp
)

# Set targer as static library
add_library(${LIB_NAME_XLSX_SERIALIZE} STATIC ${XLSX_SERIALIZE_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_XLSX_SERIALIZE}
    PRIVATE
        ${LIB_NAME_FONT_ENGINE}
)

# Add include files .h
target_include_directories(${LIB_NAME_XLSX_SERIALIZE}
    PRIVATE
        ${XLSX_SERIALIZE_DIR}Common/
        ${XLSX_SERIALIZE_DIR}Reader/
        ${XLSX_SERIALIZE_DIR}Writer/
        ${XLSX_SERIALIZE_DIR}Common/1/
)

# Set target definition
target_compile_definitions(${LIB_NAME_XLSX_SERIALIZE}
    PRIVATE
        _UNICODE
        UNICODE
        DONT_WRITE_EMBEDDED_FONTS
)