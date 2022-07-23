QT       -= core
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/../../../../core
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

############### destination path ###############
DESTDIR = $$PWD/build
################################################

ADD_DEPENDENCY(graphics, kernel, kernel_network, UnicodeConverter, doctrenderer)

core_linux {
    LIBS += -ldl
}

SOURCES += main.cpp
