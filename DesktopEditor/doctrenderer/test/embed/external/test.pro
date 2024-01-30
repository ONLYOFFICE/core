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
include($$CORE_ROOT_DIR/DesktopEditor/doctrenderer/js_internal/js_base_embed.pri)

############### destination path ###############
DESTDIR = $$PWD/build
################################################
INCLUDEPATH += $$CORE_ROOT_DIR/DesktopEditor/doctrenderer

DEFINES += CURR_DIR=\\\"$$PWD_ROOT_DIR\\\"

ADD_DEPENDENCY(doctrenderer)

core_linux {
    LIBS += -Wl,-unresolved-symbols=ignore-in-shared-libs
    LIBS += -ldl
}

SOURCES += main.cpp \
    Embed.cpp

HEADERS += \
    Embed.h

ADD_FILES_FOR_EMBEDDED_CLASS_HEADER(Embed.h)
