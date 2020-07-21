QT       -= core
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(gumbo)

INCLUDEPATH += $$PWD/../gumbo-parser/src
SOURCES += $$PWD/../gumbo-parser/examples/prettyprint.cc

DESTDIR = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX
