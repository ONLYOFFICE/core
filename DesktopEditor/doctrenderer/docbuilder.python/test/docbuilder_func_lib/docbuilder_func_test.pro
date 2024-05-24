QT		-= core
QT		-= gui

TARGET	 = test

TEMPLATE = app

CONFIG -= app_bundle
CONFIG += console

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

# Set docbuilder path here
DOCUMENT_BUILDER_INSTALL_PATH=C:/Program Files/ONLYOFFICE/DocumentBuilder

DEFINES += "DOCUMENT_BUILDER_INSTALL_PATH=\"$$DOCUMENT_BUILDER_INSTALL_PATH\""
LIBS += -L'$$DOCUMENT_BUILDER_INSTALL_PATH' -ldocbuilder.c

DESTDIR = $$PWD/build

INCLUDEPATH += ../../..

SOURCES += \
	main.cpp
