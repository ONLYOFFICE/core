DEFINES += HAVE_VA_COPY

INCLUDEPATH += \
    $$LIB_XML_PRI_PATH/libxml2/include \
    $$LIB_XML_PRI_PATH/libxml2/include/libxml \
    $$LIB_XML_PRI_PATH/build/qt


SOURCES +=  \
    $$LIB_XML_PRI_PATH/libxml2/buf.c \
    $$LIB_XML_PRI_PATH/libxml2/c14n.c \
    $$LIB_XML_PRI_PATH/libxml2/catalog.c \
    $$LIB_XML_PRI_PATH/libxml2/chvalid.c \
    $$LIB_XML_PRI_PATH/libxml2/debugXML.c \
    $$LIB_XML_PRI_PATH/libxml2/dict.c \
    $$LIB_XML_PRI_PATH/libxml2/DOCBparser.c \
    $$LIB_XML_PRI_PATH/libxml2/encoding.c \
    $$LIB_XML_PRI_PATH/libxml2/entities.c \
    $$LIB_XML_PRI_PATH/libxml2/error.c \
    $$LIB_XML_PRI_PATH/libxml2/globals.c \
    $$LIB_XML_PRI_PATH/libxml2/HTMLparser.c \
    $$LIB_XML_PRI_PATH/libxml2/HTMLtree.c \
    $$LIB_XML_PRI_PATH/libxml2/legacy.c \
    $$LIB_XML_PRI_PATH/libxml2/list.c \
    $$LIB_XML_PRI_PATH/libxml2/nanoftp.c \
    $$LIB_XML_PRI_PATH/libxml2/nanohttp.c \
    $$LIB_XML_PRI_PATH/libxml2/parser.c \
    $$LIB_XML_PRI_PATH/libxml2/pattern.c \
    $$LIB_XML_PRI_PATH/libxml2/relaxng.c \
    $$LIB_XML_PRI_PATH/libxml2/SAX.c \
    $$LIB_XML_PRI_PATH/libxml2/SAX2.c \
    $$LIB_XML_PRI_PATH/libxml2/schematron.c \
    $$LIB_XML_PRI_PATH/libxml2/threads.c \
    $$LIB_XML_PRI_PATH/libxml2/tree.c \
    $$LIB_XML_PRI_PATH/libxml2/uri.c \
    $$LIB_XML_PRI_PATH/libxml2/valid.c \
    $$LIB_XML_PRI_PATH/libxml2/xinclude.c \
    $$LIB_XML_PRI_PATH/libxml2/xlink.c \
    $$LIB_XML_PRI_PATH/libxml2/xmlIO.c \
    $$LIB_XML_PRI_PATH/libxml2/xmlmemory.c \
    $$LIB_XML_PRI_PATH/libxml2/xmlmodule.c \
    $$LIB_XML_PRI_PATH/libxml2/xmlreader.c \
    $$LIB_XML_PRI_PATH/libxml2/xmlregexp.c \
    $$LIB_XML_PRI_PATH/libxml2/xmlsave.c \
    $$LIB_XML_PRI_PATH/libxml2/xmlschemas.c \
    $$LIB_XML_PRI_PATH/libxml2/xmlschemastypes.c \
    $$LIB_XML_PRI_PATH/libxml2/xmlstring.c \
    $$LIB_XML_PRI_PATH/libxml2/xmlunicode.c \
    $$LIB_XML_PRI_PATH/libxml2/xmlwriter.c \
    $$LIB_XML_PRI_PATH/libxml2/xpath.c \
    $$LIB_XML_PRI_PATH/libxml2/xpointer.c \
    $$LIB_XML_PRI_PATH/libxml2/parserInternals.c \
    $$LIB_XML_PRI_PATH/libxml2/hash.c

SOURCES +=  \
    $$LIB_XML_PRI_PATH/src/xmllight.cpp \
    $$LIB_XML_PRI_PATH/src/xmldom.cpp
