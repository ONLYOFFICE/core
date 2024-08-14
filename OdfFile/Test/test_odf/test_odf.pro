TEMPLATE = app

QT       -= core
QT       -= gui

TARGET = test_odf
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += core_static_link_libstd
CONFIG += build_x2t_as_library

CORE_ROOT_DIR = $$PWD/../../../../core

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/googletest/googletest.pri)
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)

LIBS += -L$$CORE_BOOST_LIBS

INCLUDEPATH += $$CORE_ROOT_DIR/X2tConverter/src/

DESTDIR = $$PWD/build  

HEADERS += \
    common.h
#    entrance.h\
#    motion.h\
#    audio.h\
#   interactions.h

SOURCES += \
    test.cpp\
    common.cpp
#    entrance.cpp\
#    motion.cpp\
#    audio.cpp\
#    interactions.cpp

SOURCES -= $$CORE_GTEST_PATH/src/gtest_main.cc
