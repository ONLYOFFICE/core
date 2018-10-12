TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    $$PWD/../../../../core/Common/OfficeFileFormatChecker2.cpp \
    $$PWD/../../../../core/Common/3dParty/pole/pole.cpp \
    $$PWD/../../../../core/Common/DocxFormat/Source/Base/unicode_util.cpp

SOURCES += main.cpp

CORE_ROOT_DIR = $$PWD/../../../../core
PWD_ROOT_DIR = $$PWD
include($$PWD/../../../Common/base.pri)

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lkernel -lgraphics

DESTDIR = $$PWD/build
