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
#CONFIG += v8_version_89
include($$CORE_ROOT_DIR/DesktopEditor/doctrenderer/js_internal/js_base.pri)

############### destination path ###############
DESTDIR = $$PWD/build
################################################

ADD_DEPENDENCY(kernel, UnicodeConverter)

core_linux {
    LIBS += -Wl,-unresolved-symbols=ignore-in-shared-libs
    LIBS += -ldl
}

SOURCES += main.cpp

# cout logs
DEFINES += LOG_TO_COUT
