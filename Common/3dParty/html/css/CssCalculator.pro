#CONFIG -= qt

#TEMPLATE = lib
#DEFINES += CSSCALCULATOR_LIBRARY

#CONFIG += c++11

#DEFINES += QT_DEPRECATED_WARNINGS

QT       -= core gui

VERSION = 1.0.0.0
TARGET = CssCalculator
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += core_static_link_libstd

DEFINES += CSSCALCULATOR_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/html/katana.pri)

ADD_DEPENDENCY(kernel, UnicodeConverter)

HEADERS += \
    src/CCompiledStyle.h \
    src/CCssCalculator.h \
    src/CCssCalculator_Private.h \
    src/CElement.h \
    src/CssCalculator_global.h

SOURCES += \
    src/CCompiledStyle.cpp \
    src/CCssCalculator.cpp \
    src/CCssCalculator_Private.cpp \
    src/CElement.cpp


