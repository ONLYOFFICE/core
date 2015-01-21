#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T14:24:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = libxml2
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers -g
DEFINES += UNICODE \
    _UNICODE \
    LINUX \
    _LINUX_QT \
    _USE_LIBXML2_READER_ \
    LIBXML_READER_ENABLED \
    LIBXML_PUSH_ENABLED

INCLUDEPATH += \
    ../XML/include \
    ../XML/include/libxml

SOURCES +=  \
    ../XML/buf.c \
    ../XML/c14n.c \
    ../XML/catalog.c \
    ../XML/chvalid.c \
    ../XML/debugXML.c \
    ../XML/dict.c \
    ../XML/DOCBparser.c \
    ../XML/encoding.c \
    ../XML/entities.c \
    ../XML/error.c \
    ../XML/globals.c \
    ../XML/hash.c \
    ../XML/HTMLparser.c \
    ../XML/HTMLtree.c \
    ../XML/legacy.c \
    ../XML/list.c \
    ../XML/nanoftp.c \
    ../XML/nanohttp.c \
    ../XML/parser.c \
    ../XML/parserInternals.c \
    ../XML/pattern.c \
    ../XML/relaxng.c \
    ../XML/SAX.c \
    ../XML/SAX2.c \
    ../XML/schematron.c \
    ../XML/threads.c \
    ../XML/tree.c \
    ../XML/uri.c \
    ../XML/valid.c \
    ../XML/xinclude.c \
    ../XML/xmlcatalog.c \
    ../XML/xlink.c \
    ../XML/xmlIO.c \
    ../XML/xmlmemory.c \
    ../XML/xmlmodule.c \
    ../XML/xmlreader.c \
    ../XML/xmlregexp.c \
    ../XML/xmlsave.c \
    ../XML/xmlschemas.c \
    ../XML/xmlschemastypes.c \
    ../XML/xmlstring.c \
    ../XML/xmlunicode.c \
    ../XML/xmlwriter.c \
    ../XML/xpath.c \
    ../XML/xpointer.c \
	
	
HEADERS +=  \
    ../XML/elfgcchack.h \
    ../XML/libxml.h \
	

unix {
    target.path = /usr/lib
    INSTALLS += target

    DEFINES += HAVE_VA_COPY
}
