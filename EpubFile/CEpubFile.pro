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

ADD_DEPENDENCY(kernel, graphics, HtmlFile2)

SOURCES += src/CEpubFile.cpp \
    src/CBookToc.cpp \
    src/CBookContentItem.cpp \
    src/CBookItem.cpp \
    src/CBookInfo.cpp \

HEADERS += CEpubFile.h \
    src/CBookToc.h \
    src/CBookContentItem.h \
    src/CBookItem.h \
    src/CBookInfo.h \
    src/EpubFile_global.h

