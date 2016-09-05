DEFINES += HAVE_VA_COPY

INCLUDEPATH += \
    $$PWD/../../libxml2/include \
    $$PWD/../../libxml2/include/libxml \
    $$PWD

SOURCES += $$PWD/libxml2_all.c
SOURCES += $$PWD/libxml2_all2.c

SOURCES +=  \
    $$PWD/../../src/xmllight.cpp \
    $$PWD/../../src/xmldom.cpp
