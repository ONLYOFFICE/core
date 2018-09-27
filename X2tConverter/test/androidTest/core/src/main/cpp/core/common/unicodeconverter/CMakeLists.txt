cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED UNICODE_CONVERTER_DIR)
    message(FATAL_ERROR "You must set path in \"UNICODE_CONVERTER_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_UNICODE_CONVERTER)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_UNICODE_CONVERTER\"!")
endif()

# Library source .h .cpp
file(GLOB UNICODE_CONVERTER_CPP ${UNICODE_CONVERTER_DIR}*.cpp)

# Set targer as static library
add_library(${LIB_NAME_UNICODE_CONVERTER} STATIC ${UNICODE_CONVERTER_CPP})

# Add include files .h
target_include_directories(${LIB_NAME_UNICODE_CONVERTER} PUBLIC ${UNICODE_CONVERTER_DIR})

# Add dependency library
target_link_libraries(${LIB_NAME_UNICODE_CONVERTER}
    PUBLIC
        ${LIB_NAME_ICU_UC}
        ${LIB_NAME_ICU_DATA}
        ${LIB_NAME_ICU_i18n}
)

# Set target definition
target_compile_definitions(${LIB_NAME_UNICODE_CONVERTER}
    PRIVATE
        UNICODECONVERTER_USE_DYNAMIC_LIBRARY
        U_COMMON_IMPLEMENTATION
        U_I18N_IMPLEMENTATION
        U_IO_IMPLEMENTATION
)