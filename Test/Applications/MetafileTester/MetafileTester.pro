TARGET    = metafiletester
CONFIG   += console c++14
CONFIG   -= app_bundle
CONFIG   -= qt
TEMPLATE  = app

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)

SOURCES += main.cpp

DESTDIR = $$CORE_BUILDS_BINARY_PATH
