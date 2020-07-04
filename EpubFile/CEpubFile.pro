QT       -= core gui

VERSION = 1.0.0.0
TARGET = EpubFile
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += core_static_link_libstd

DEFINES += EPUBFILE_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel)


SOURCES += src/CEpubFile.cpp

HEADERS += CEpubFile.h \
    src/EpubFile_global.h

