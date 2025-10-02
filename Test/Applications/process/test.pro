TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

CONFIG += core_boost_libs
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)

ADD_DEPENDENCY(kernel)

SOURCES += \
        main.cpp

DESTDIR = $$PWD/build
