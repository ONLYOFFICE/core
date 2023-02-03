QT       -= core
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/../../../../../core
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/DesktopEditor/doctrenderer/js_internal/js_base.pri)

############### destination path ###############
DESTDIR = $$PWD/build
################################################

ADD_DEPENDENCY(graphics, kernel, kernel_network, UnicodeConverter, doctrenderer)

core_linux {
#    include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
    LIBS += -Wl,-unresolved-symbols=ignore-in-shared-libs
    LIBS += -ldl
}

SOURCES +=  \
    ../../embed/ZipEmbed.cpp \
    ../../embed/v8/v8_Zip.cpp

HEADERS +=  \
    ../../embed/ZipEmbed.h

SOURCES += main.cpp

DEFINES += CURR_DIR=\\\"$$PWD_ROOT_DIR\\\"
