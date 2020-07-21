QT       -= core
QT       -= gui

VERSION = 1.0.0.3
TARGET = katana
TEMPLATE = lib
CONFIG += staticlib

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

core_windows:INCLUDEPATH += $$PWD/katana-parser/visualc/include

HEADERS += $$files($$PWD/katana-parser/src/*.h, true)
SOURCES += $$files($$PWD/katana-parser/src/*.c, true)
