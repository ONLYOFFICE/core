cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED COMMON_EDITOR_DIR)
    message(FATAL_ERROR "You must set path in \"COMMON_EDITOR_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_EDITOR_COMMON)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_EDITOR_COMMON\"!")
endif()

# Library source .h .cpp
file(GLOB COMMON_EDITOR_CPP ${COMMON_EDITOR_DIR}*.cpp)

# Set targer as static library
add_library(${LIB_NAME_EDITOR_COMMON} STATIC ${COMMON_EDITOR_CPP})

# Set target definition
target_compile_definitions(${LIB_NAME_EDITOR_COMMON}
    PRIVATE _LINUX
)