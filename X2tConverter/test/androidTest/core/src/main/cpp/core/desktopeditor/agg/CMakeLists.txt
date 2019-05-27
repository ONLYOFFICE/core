cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED AGG_DIR)
    message(FATAL_ERROR "You must set path in \"AGG_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_AGG)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_AGG\"!")
endif()

# Library source .h .cpp
file(GLOB AGG_CPP ${AGG_DIR}src/*.cpp)

# Set targer as static library
add_library(${LIB_NAME_AGG} STATIC ${AGG_CPP})

# Add include files .h
target_include_directories(${LIB_NAME_AGG} PUBLIC ${AGG_DIR}include/)

# Add compile options
target_compile_options(${LIB_NAME_AGG}
    PUBLIC
        -Wno-register
)