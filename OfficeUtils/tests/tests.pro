QT       -= core
QT       -= gui


TARGET = tests
CONFIG   += console
CONFIG   -= app_bundle
CONFIG -= qt

TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../../
PWD_ROOT_DIR = $$PWD

SOURCES += \
$$PWD/../../Common/OfficeFileFormatChecker2.cpp \
$$PWD/../../Common/3dParty/pole/pole.cpp \
$$PWD/../../OOXML/Base/unicode_util.cpp

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

DEFINES += PRO_DIR=\\\"$$PWD/\\\"
ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)

SOURCES += main.cpp \
    main.cpp

DESTDIR = $$PWD/build
