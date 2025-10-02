CONFIG -= qt
QT -= core gui

TARGET   = TxtRendererTest
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(TxtXmlFormatLib, PdfFile, graphics, kernel)

SOURCES += \
    main.cpp
