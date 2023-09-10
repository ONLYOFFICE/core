QT -= core gui

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(UnicodeConverter, kernel)

SOURCES += Test.cpp \
    cconversionsmtoooxml.cpp \
    cstarmathpars.cpp

HEADERS += \
    cconversionsmtoooxml.h \
    cstarmathpars.h \
    typeselements.h

DESTDIR = $$PWD_ROOT_DIR/build/$$CIRE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX
