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

SOURCES += $$PWD/libxml2_all.c
SOURCES += $$PWD/libxml2_all2.c

SOURCES +=  \
    $$PWD/../../src/xmlwriter.cpp \
    $$PWD/../../src/xmllight.cpp \
    $$PWD/../../src/xmldom.cpp

HEADERS += \
    $$PWD/../../src/xmllight_private.h

HEADERS += \
    $$PWD/../../include/xmlutils.h \
    $$PWD/../../include/xmlwriter.h
