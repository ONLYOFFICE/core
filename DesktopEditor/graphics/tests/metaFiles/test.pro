CONFIG += c++11

QT       -= core gui

SOURCES += \
	main.cpp\

PWD_ROOT_DIR = $$PWD
CORE_ROOT_DIR = $$PWD/../../../../../core
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
