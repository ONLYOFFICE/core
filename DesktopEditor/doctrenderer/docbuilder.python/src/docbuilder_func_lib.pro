QT		-= core
QT		-= gui

TARGET	 = docbuilder_functions

TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(doctrenderer)

DESTDIR = $$PWD/build

INCLUDEPATH += ../..

DEFINES += DOCBUILDER_FUNCTIONS_EXPORT

SOURCES += \
	docbuilder_functions.cpp

HEADERS += \
	docbuilder_functions.h
