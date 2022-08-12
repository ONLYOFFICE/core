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

############### destination path ###############
DESTDIR = $$PWD/build
################################################

# Use installation builder path, or set the own path.
core_windows {
    DOCUMENT_BUILDER_INSTALL_PATH="C:/Program Files/ONLYOFFICE/DocumentBuilder"
} else {
    DOCUMENT_BUILDER_INSTALL_PATH=/opt/onlyoffice/documentbuilder
}

DEFINES += "DOCUMENT_BUILDER_INSTALL_PATH=\"$$DOCUMENT_BUILDER_INSTALL_PATH\""
LIBS += -L'$$DOCUMENT_BUILDER_INSTALL_PATH' -ldoctrenderer

core_linux {
    include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
    LIBS += -L'$$DOCUMENT_BUILDER_INSTALL_PATH' -lUnicodeConverter -lkernel -lkernel_network -lgraphics
    LIBS += -ldl
}

SOURCES += main.cpp
