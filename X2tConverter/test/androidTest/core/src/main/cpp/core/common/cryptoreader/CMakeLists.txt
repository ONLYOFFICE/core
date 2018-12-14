cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

# Lib src path
if (NOT DEFINED CRYPTO_READER_DIR)
    message(FATAL_ERROR "You must set path in \"CRYPTO_READER_DIR\"!")
endif()

# Lib name
if (NOT DEFINED LIB_NAME_CRYPTO_READER)
    message(FATAL_ERROR "You must set library name in \"LIB_NAME_CRYPTO_READER\"!")
endif()

# Library source .h .cpp
file(GLOB CRYPTO_READER_CPP
    ${CRYPTO_READER_DIR}source/*.cpp
)

# Set targer as static library
add_library(${LIB_NAME_CRYPTO_READER} STATIC ${CRYPTO_READER_CPP})

# Add dependency library
target_link_libraries(${LIB_NAME_CRYPTO_READER}
    PRIVATE
        ${LIB_NAME_CRYPTOPP}
)

# Add include files .h
target_include_directories(${LIB_NAME_CRYPTO_READER}
    PUBLIC
        ${CRYPTO_READER_DIR}source/
)
