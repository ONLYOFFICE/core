QT       -= core
QT       -= gui

TARGET = test
TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/html/css/CssCalculator.pri)
include($$CORE_ROOT_DIR/Common/3dParty/html/katana.pri)


DESTDIR = $$PWD_ROOT_DIR/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX

ADD_DEPENDENCY(kernel, UnicodeConverter)

SOURCES += \
    test.cpp
