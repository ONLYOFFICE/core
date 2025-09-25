include(../../../../Common/3dParty/googletest/googletest.pri)

TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

SOURCES += \
        tst_booleanoperations.cpp

PWD_ROOT_DIR = $$PWD
CORE_ROOT_DIR = $$PWD/../../../../../core
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
