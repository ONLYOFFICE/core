DEFINES += HAVE_VA_COPY

INCLUDEPATH += \
    ../DesktopEditor/xml/libxml2/include \
    ../DesktopEditor/xml/libxml2/include/libxml \
    ../DesktopEditor/xml/build/qt


SOURCES +=  \
    ../DesktopEditor/xml/libxml2/buf.c \
    ../DesktopEditor/xml/libxml2/c14n.c \
    ../DesktopEditor/xml/libxml2/catalog.c \
    ../DesktopEditor/xml/libxml2/chvalid.c \
    ../DesktopEditor/xml/libxml2/debugXML.c \
    ../DesktopEditor/xml/libxml2/dict.c \
    ../DesktopEditor/xml/libxml2/DOCBparser.c \
    ../DesktopEditor/xml/libxml2/encoding.c \
    ../DesktopEditor/xml/libxml2/entities.c \
    ../DesktopEditor/xml/libxml2/error.c \
    ../DesktopEditor/xml/libxml2/globals.c \
    ../DesktopEditor/xml/libxml2/HTMLparser.c \
    ../DesktopEditor/xml/libxml2/HTMLtree.c \
    ../DesktopEditor/xml/libxml2/legacy.c \
    ../DesktopEditor/xml/libxml2/list.c \
    ../DesktopEditor/xml/libxml2/nanoftp.c \
    ../DesktopEditor/xml/libxml2/nanohttp.c \
    ../DesktopEditor/xml/libxml2/parser.c \
    ../DesktopEditor/xml/libxml2/pattern.c \
    ../DesktopEditor/xml/libxml2/relaxng.c \
    ../DesktopEditor/xml/libxml2/SAX.c \
    ../DesktopEditor/xml/libxml2/SAX2.c \
    ../DesktopEditor/xml/libxml2/schematron.c \
    ../DesktopEditor/xml/libxml2/threads.c \
    ../DesktopEditor/xml/libxml2/tree.c \
    ../DesktopEditor/xml/libxml2/uri.c \
    ../DesktopEditor/xml/libxml2/valid.c \
    ../DesktopEditor/xml/libxml2/xinclude.c \
    ../DesktopEditor/xml/libxml2/xlink.c \
    ../DesktopEditor/xml/libxml2/xmlIO.c \
    ../DesktopEditor/xml/libxml2/xmlmemory.c \
    ../DesktopEditor/xml/libxml2/xmlmodule.c \
    ../DesktopEditor/xml/libxml2/xmlreader.c \
    ../DesktopEditor/xml/libxml2/xmlregexp.c \
    ../DesktopEditor/xml/libxml2/xmlsave.c \
    ../DesktopEditor/xml/libxml2/xmlschemas.c \
    ../DesktopEditor/xml/libxml2/xmlschemastypes.c \
    ../DesktopEditor/xml/libxml2/xmlstring.c \
    ../DesktopEditor/xml/libxml2/xmlunicode.c \
    ../DesktopEditor/xml/libxml2/xmlwriter.c \
    ../DesktopEditor/xml/libxml2/xpath.c \
    ../DesktopEditor/xml/libxml2/xpointer.c \
    ../DesktopEditor/xml/libxml2/parserInternals.c \
    ../DesktopEditor/xml/libxml2/hash.c

SOURCES +=  \
    ../DesktopEditor/xml/src/xmllight.cpp \
    ../DesktopEditor/xml/src/xmldom.cpp
	
	
HEADERS +=  \
    ../XML/elfgcchack.h \
    ../XML/libxml.h
	

