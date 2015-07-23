#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T14:24:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = libxml
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++11 -Wall -g
DEFINES += HAVE_VA_COPY

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../../../SDK/lib

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_64
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_32
}

################################################

INCLUDEPATH += \
    ../../libxml2/include \
    ../../libxml2/include/libxml

SOURCES +=  \
    ../../libxml2/buf.c \
    ../../libxml2/c14n.c \
    ../../libxml2/catalog.c \
    ../../libxml2/chvalid.c \
    ../../libxml2/debugXML.c \
    ../../libxml2/dict.c \
    ../../libxml2/DOCBparser.c \
    ../../libxml2/encoding.c \
    ../../libxml2/entities.c \
    ../../libxml2/error.c \
    ../../libxml2/globals.c \
    ../../libxml2/HTMLparser.c \
    ../../libxml2/HTMLtree.c \
    ../../libxml2/legacy.c \
    ../../libxml2/list.c \
    ../../libxml2/nanoftp.c \
    ../../libxml2/nanohttp.c \
    ../../libxml2/parser.c \
    ../../libxml2/pattern.c \
    ../../libxml2/relaxng.c \
    ../../libxml2/SAX.c \
    ../../libxml2/SAX2.c \
    ../../libxml2/schematron.c \
    ../../libxml2/threads.c \
    ../../libxml2/tree.c \
    ../../libxml2/uri.c \
    ../../libxml2/valid.c \
    ../../libxml2/xinclude.c \
    ../../libxml2/xlink.c \
    ../../libxml2/xmlIO.c \
    ../../libxml2/xmlmemory.c \
    ../../libxml2/xmlmodule.c \
    ../../libxml2/xmlreader.c \
    ../../libxml2/xmlregexp.c \
    ../../libxml2/xmlsave.c \
    ../../libxml2/xmlschemas.c \
    ../../libxml2/xmlschemastypes.c \
    ../../libxml2/xmlstring.c \
    ../../libxml2/xmlunicode.c \
    ../../libxml2/xmlwriter.c \
    ../../libxml2/xpath.c \
    ../../libxml2/xpointer.c \
    ../../libxml2/parserInternals.c \
    ../../libxml2/hash.c

SOURCES +=  \
    ../../src/xmllight.cpp \
    ../../src/xmldom.cpp
	
	
HEADERS +=  \
    ../XML/elfgcchack.h \
    ../XML/libxml.h \
	

unix {
    target.path = /usr/lib
    INSTALLS += target
}
