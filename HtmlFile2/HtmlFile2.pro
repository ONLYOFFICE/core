QT -= core
QT -= gui

TARGET = HtmlFile2
TEMPLATE = lib
CONFIG += staticlib

DEFINES += HTMLFILE2_USE_DYNAMIC_LIBRARY
DEFINES += UNICODE \
          _UNICODE \
           DONT_WRITE_EMBEDDED_FONTS

CORE_ROOT_DIR = $$PWD/../../core
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

#BOOST
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)

ADD_DEPENDENCY(kernel, gumbo, UnicodeConverter, graphics, DocxFormatLib)

SOURCES += htmlfile2.cpp

HEADERS += htmlfile2.h
