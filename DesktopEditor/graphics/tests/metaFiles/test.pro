CONFIG += c++11

QMAKE_CXXFLAGS += /permissive-

QT       += core gui widgets

SOURCES += \
	main.cpp\
	mainwindow.cpp

FORMS += \
	mainwindow.ui

HEADERS += \
	mainwindow.h

PWD_ROOT_DIR = $$PWD
CORE_ROOT_DIR = $$PWD/../../../../../core
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

