TARGET = test
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt
CONFIG += core_no_gtest_main core_gmock

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD
include(../../base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/googletest/googletest.pri)

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
