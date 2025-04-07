CONFIG -= qt
TARGET = test
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

CORE_ROOT_DIR = $$PWD/../../../../../core
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

include($$CORE_ROOT_DIR/DesktopEditor/graphics/pro/freetype.pri)
include($$CORE_ROOT_DIR/Common/3dParty/harfbuzz/harfbuzz.pri)

SOURCES += main.cpp

ADD_DEPENDENCY(UnicodeConverter, kernel, graphics)
DESTDIR = $$PWD/build


