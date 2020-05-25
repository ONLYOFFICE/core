QT       -= core
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../../../../../core
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

############### destination path ###############
DESTDIR = $$PWD_ROOT_DIR/build/$$CORE_BUILDS_PLATFORM_PREFIX
################################################

INCLUDEPATH += $$PWD_ROOT_DIR/../build/$$CORE_BUILDS_PLATFORM_PREFIX/include
LIBS += -L$$PWD_ROOT_DIR/../build/$$CORE_BUILDS_PLATFORM_PREFIX/lib -llibcrypto

core_windows:LIBS += -lws2_32 -lAdvapi32 -lCrypt32 -lUser32

ADD_DEPENDENCY(kernel)

HEADERS += $$PWD_ROOT_DIR/../common/common_openssl.h
SOURCES += $$PWD_ROOT_DIR/../common/common_openssl.cpp

SOURCES += main.cpp
