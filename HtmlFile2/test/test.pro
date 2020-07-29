QT -= core
QT -= gui

TARGET = test
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

DEFINES += UNICODE \
          _UNICODE \
           DONT_WRITE_EMBEDDED_FONTS

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

SOURCES += main.cpp

CORE_ROOT_DIR = $$PWD/../../../core
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lHtmlFile2

ADD_DEPENDENCY(ASCOfficeDocxFile2Lib, PPTXFormatLib, DocxFormatLib, CryptoPPLib)
ADD_DEPENDENCY(kernel, gumbo, UnicodeConverter, graphics, HtmlFile2)

#BOOST
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)

DESTDIR = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX
