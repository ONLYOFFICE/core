QT       -= core
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/../../../../../../core
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

############### destination path ###############
DESTDIR = $$PWD/build
################################################

ADD_DEPENDENCY(kernel, UnicodeConverter, doctrenderer)

core_linux {
    LIBS += -Wl,-unresolved-symbols=ignore-in-shared-libs
    LIBS += -ldl
}

SOURCES += main.cpp

# cout logs
DEFINES += LOG_TO_COUT
