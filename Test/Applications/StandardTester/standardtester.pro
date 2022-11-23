TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

TARGET = standardtester

SOURCES += \
    $$PWD/../../../../core/Common/OfficeFileFormatChecker2.cpp \
    $$PWD/../../../../core/Common/3dParty/pole/pole.cpp \
    $$PWD/../../../../core/OOXML/Base/unicode_util.cpp

SOURCES += main.cpp

CORE_ROOT_DIR = $$PWD/../../../../core
PWD_ROOT_DIR = $$PWD
include($$PWD/../../../Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)

DESTDIR = $$CORE_BUILDS_BINARY_PATH
