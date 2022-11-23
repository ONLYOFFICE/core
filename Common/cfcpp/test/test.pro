include(gtest_dependency.pri)

TARGET = test
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD
include(../../base.pri)

ADD_DEPENDENCY(UnicodeConverter, kernel, CompoundFileLib)

INCLUDEPATH += $$PWD/../

HEADERS += \
    global.h \
    tst_compondfile.h \
    tst_data_set.h \
    tst_directoryentry.h \
    tst_header.h \
    tst_streamrw.h

SOURCES += \
        main.cpp
