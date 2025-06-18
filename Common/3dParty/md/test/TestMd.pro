QT -= core
QT -= gui

TARGET = test
TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

CORE_ROOT_DIR = $$PWD/../../../../

SOURCES += main.cpp

include($$CORE_ROOT_DIR/Common/3dParty/md/md2html.pri)

DESTDIR = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX
