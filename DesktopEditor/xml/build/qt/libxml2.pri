DEFINES += HAVE_VA_COPY

core_static_link_xml_full {
DEFINES += \
    LIBXML_READER_ENABLED \
    LIBXML_PUSH_ENABLED \
    LIBXML_HTML_ENABLED \
    LIBXML_XPATH_ENABLED \
    LIBXML_OUTPUT_ENABLED \
    LIBXML_C14N_ENABLED \
    LIBXML_SAX1_ENABLED \
    LIBXML_TREE_ENABLED \
    LIBXML_XPTR_ENABLED \
    IN_LIBXML \
    LIBXML_STATIC
}

INCLUDEPATH += \
    $$PWD/../../libxml2/include \
    $$PWD/../../libxml2/include/libxml \
    $$PWD

core_release {
    SOURCES += $$PWD/libxml2_all.c
    SOURCES += $$PWD/libxml2_all2.c

    DEFINES += XML_ERROR_DISABLE_MODE
}

core_debug {
SOURCES += \
    $$PWD/../../libxml2/buf.c \
    $$PWD/../../libxml2/c14n.c \
    $$PWD/../../libxml2/catalog.c \
    $$PWD/../../libxml2/chvalid.c \
    $$PWD/../../libxml2/debugXML.c \
    $$PWD/../../libxml2/dict.c \
    $$PWD/../../libxml2/DOCBparser.c \
    $$PWD/../../libxml2/encoding.c \
    $$PWD/../../libxml2/entities.c \
    $$PWD/../../libxml2/error.c \
    $$PWD/../../libxml2/globals.c \
    $$PWD/../../libxml2/hash.c \
    $$PWD/../../libxml2/HTMLparser.c \
    $$PWD/../../libxml2/HTMLtree.c \
    $$PWD/../../libxml2/legacy.c \
    $$PWD/../../libxml2/list.c \
    $$PWD/../../libxml2/nanoftp.c \
    $$PWD/../../libxml2/nanohttp.c \
    $$PWD/../../libxml2/parserInternals.c \
    $$PWD/../../libxml2/pattern.c \
    $$PWD/../../libxml2/relaxng.c \
    $$PWD/../../libxml2/SAX.c \
    $$PWD/../../libxml2/SAX2.c \
    $$PWD/../../libxml2/schematron.c \
    $$PWD/../../libxml2/threads.c \
    $$PWD/../../libxml2/tree.c \
    $$PWD/../../libxml2/uri.c \
    $$PWD/../../libxml2/xinclude.c \
    $$PWD/../../libxml2/xlink.c \
    $$PWD/../../libxml2/xmlIO.c \
    $$PWD/../../libxml2/xmlmemory.c \
    $$PWD/../../libxml2/xmlmodule.c \
    $$PWD/../../libxml2/xmlreader.c \
    $$PWD/../../libxml2/xmlregexp.c \
    $$PWD/../../libxml2/xmlsave.c \
    $$PWD/../../libxml2/xmlschemas.c \
    $$PWD/../../libxml2/xmlschemastypes.c \
    $$PWD/../../libxml2/xmlstring.c \
    $$PWD/../../libxml2/xmlunicode.c \
    $$PWD/../../libxml2/xmlwriter.c \
    $$PWD/../../libxml2/xpath.c \
    $$PWD/../../libxml2/xpointer.c \
    $$PWD/../../libxml2/valid.c \
     $$PWD/../../libxml2/parser.c
}

!core_only_libxml {
SOURCES +=  \
    $$PWD/../../src/xmlwriter.cpp \
    $$PWD/../../src/xmllight.cpp \
    $$PWD/../../src/xmldom.cpp

HEADERS += \
    $$PWD/../../src/xmllight_private.h

HEADERS += \
    $$PWD/../../include/xmlutils.h \
    $$PWD/../../include/xmlwriter.h
}
