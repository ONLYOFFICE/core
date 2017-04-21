QT       -= core gui

TARGET = test
TEMPLATE = app

CONFIG   += console
CONFIG   -= app_bundle

CORE_ROOT_DIR = $$PWD/../../../../
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

DEFINES -= UNICODE

DEFINES += \
    LIBXML_READER_ENABLED \
    LIBXML_PUSH_ENABLED \
    LIBXML_HTML_ENABLED \
    LIBXML_XPATH_ENABLED \
    LIBXML_OUTPUT_ENABLED \
    LIBXML_C14N_ENABLED \
    LIBXML_SAX1_ENABLED \
    LIBXML_TREE_ENABLED \
    LIBXML_XPTR_ENABLED \
    LIBXML_STATIC

SOURCES += main.cpp

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -llibxml

INCLUDEPATH += \
    $$CORE_ROOT_DIR/DesktopEditor/xml/libxml2/include \
    $$CORE_ROOT_DIR/DesktopEditor/xml/libxml2/include/libxml


