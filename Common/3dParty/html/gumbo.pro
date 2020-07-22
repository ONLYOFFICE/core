QT       -= core
QT       -= gui

VERSION = 1.0.0.3
TARGET = gumbo
TEMPLATE = lib
CONFIG += staticlib

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

core_windows:INCLUDEPATH += $$PWD/gumbo-parser/visualc/include

HEADERS += $$files($$PWD/gumbo-parser/src/*.h, true)
SOURCES += $$files($$PWD/gumbo-parser/src/*.c, true)

MAKEFILE=$$PWD/makefiles/build.makefile_$$CORE_BUILDS_PLATFORM_PREFIX"
core_debug:MAKEFILE=$$join(MAKEFILE, MAKEFILE, "", "_debug_")
