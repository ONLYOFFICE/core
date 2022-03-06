QT -= core gui

TARGET = text
TEMPLATE = lib

CONFIG += staticlib

DEFINES += _LIB
DEFINES += _CRT_SECURE_NO_WARNINGS
DEFINES += FT2_BUILD_LIBRARY

DEFINES -= UNICODE
DEFINES -= _UNICODE

include(harfbuzz.pri)

SOURCES += text.cpp
