QT		-= core
QT		-= gui

TARGET	 = docbuilder.c

TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(graphics, kernel, kernel_network, UnicodeConverter, doctrenderer)

INCLUDEPATH += ../..

DEFINES += DOCBUILDER_FUNCTIONS_EXPORT

SOURCES += \
	docbuilder_functions.cpp

HEADERS += \
	docbuilder_functions.h
