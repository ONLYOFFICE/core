cmake_minimum_required(VERSION 3.5)

project(EpubFile LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(EpubFile SHARED
  EpubFile_global.h
  CEpubFile.cpp
  CEpubFile.h
)

target_compile_definitions(EpubFile PRIVATE EPUBFILE_LIBRARY)
