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
CONFIG += v8_use_inspector
include($$CORE_ROOT_DIR/DesktopEditor/doctrenderer/js_internal/js_base.pri)
DEFINES -= V8_INSPECTOR
DEFINES += V8_MY_INSPECTOR

############### destination path ###############
DESTDIR = $$PWD/build
################################################

ADD_DEPENDENCY(kernel, UnicodeConverter)

core_linux {
    LIBS += -Wl,-unresolved-symbols=ignore-in-shared-libs
    LIBS += -ldl
}

SOURCES += \
    main.cpp \
    inspector.cpp \
    v8_inspector_channel.cpp \
    v8_inspector_client.cpp \
    websocket_server.cpp

HEADERS += \
    inspector.h \
    utils.h\
    v8_inspector_channel.h \
    v8_inspector_client.h \
    websocket_server.h

DEFINES += CURR_DIR=\\\"$$PWD_ROOT_DIR\\\"
