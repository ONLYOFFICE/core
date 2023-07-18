TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

TARGET = tests

SOURCES += \
$$PWD/../../Common/OfficeFileFormatChecker2.cpp \
	$$PWD/../../Common/3dParty/pole/pole.cpp \
	$$PWD/../../OOXML/Base/unicode_util.cpp

SOURCES += main.cpp

CORE_ROOT_DIR = $$PWD/../../
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
include($$CORE_ROOT_DIR/Common/3dParty/googletest/googletest.pri)

ADD_DEPENDENCY(kernel, UnicodeConverter)

DESTDIR = $$PWD/build
