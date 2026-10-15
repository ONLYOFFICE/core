SRC_DIR = $$PWD/libxml2

DEFINES += \
    HAVE_VA_COPY \
    IN_LIBXML \
    LIBXML_STATIC

core_release:DEFINES += XML_ERROR_DISABLE_MODE

INCLUDEPATH += \
    $$PWD \
    $$PWD/include \
    $$SRC_DIR/include \
    $$SRC_DIR/include/libxml

SOURCES += \
    $$SRC_DIR/HTMLparser.c \
    $$SRC_DIR/HTMLtree.c \
    $$SRC_DIR/SAX2.c \
    $$SRC_DIR/buf.c \
    $$SRC_DIR/c14n.c \
    $$SRC_DIR/catalog.c \
    $$SRC_DIR/chvalid.c \
    $$SRC_DIR/debugXML.c \
    $$SRC_DIR/dict.c \
    $$SRC_DIR/encoding.c \
    $$SRC_DIR/entities.c \
    $$SRC_DIR/error.c \
    $$SRC_DIR/globals.c \
    $$SRC_DIR/hash.c \
    $$SRC_DIR/list.c \
    $$SRC_DIR/nanohttp.c \
    $$SRC_DIR/parser.c \
    $$SRC_DIR/parserInternals.c \
    $$SRC_DIR/pattern.c \
    $$SRC_DIR/relaxng.c \
    $$SRC_DIR/schematron.c \
    $$SRC_DIR/threads.c \
    $$SRC_DIR/tree.c \
    $$SRC_DIR/uri.c \
    $$SRC_DIR/valid.c \
    $$SRC_DIR/xinclude.c \
    $$SRC_DIR/xlink.c \
    $$SRC_DIR/xmlIO.c \
    $$SRC_DIR/xmlmemory.c \
    $$SRC_DIR/xmlmodule.c \
    $$SRC_DIR/xmlreader.c \
    $$SRC_DIR/xmlregexp.c \
    $$SRC_DIR/xmlsave.c \
    $$SRC_DIR/xmlschemas.c \
    $$SRC_DIR/xmlschemastypes.c \
    $$SRC_DIR/xmlstring.c \
    $$SRC_DIR/xmlwriter.c \
    $$SRC_DIR/xpath.c \
    $$SRC_DIR/xpointer.c

core_windows:LIBS += -lbcrypt
