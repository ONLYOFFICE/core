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

# get path to JDK from environment variable JAVA_HOME (preferable way) or take the default one
JDK_PATH = $$(JAVA_HOME)
isEmpty(JDK_PATH) {
	core_windows:JDK_PATH = "C:/Program Files/Java/jdk1.8.0_202"
	core_linux:JDK_PATH = "/usr/lib/jvm/java-8-openjdk-amd64"
	core_mac:JDK_PATH = "/Library/Java/JavaVirtualMachines/jdk-1.8.jdk/Contents/Home"
}

INCLUDEPATH += $$JDK_PATH/include

core_windows:JAVA_ARCH = win32
core_linux:JAVA_ARCH = linux
core_mac:JAVA_ARCH = darwin

INCLUDEPATH += $$JDK_PATH/include/$$JAVA_ARCH

SOURCES += \
	docbuilder_CDocBuilderValue.cpp \
	docbuilder_CDocBuilder.cpp \
	docbuilder_CDocBuilderContextScope.cpp \
	docbuilder_CDocBuilderContext.cpp

HEADERS += \
	docbuilder_CDocBuilderValue.h \
	docbuilder_CDocBuilder.h \
	docbuilder_CDocBuilderContextScope.h \
	docbuilder_CDocBuilderContext.h
