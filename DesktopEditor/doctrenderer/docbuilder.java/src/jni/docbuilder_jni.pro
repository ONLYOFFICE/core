QT		-= core
QT		-= gui

TARGET	 = docbuilder.jni

TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(graphics, kernel, kernel_network, UnicodeConverter, doctrenderer)

INCLUDEPATH += ../../..

# Specify JDK path here
JDK_PATH = "C:/Program Files/Java/jdk-22"

INCLUDEPATH += $$JDK_PATH/include

core_windows:JAVA_ARCH = win32
core_linux:JAVA_ARCH = linux
core_mac:JAVA_ARCH = darwin

INCLUDEPATH += $$JDK_PATH/include/$$JAVA_ARCH



SOURCES += \
	docbuilder_CDocBuilderValue.cpp

HEADERS += \
	docbuilder_CDocBuilderValue.h
