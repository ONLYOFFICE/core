
CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(UnicodeConverter, kernel)

SOURCES +=  $$PWD/cconversionsmtoooxml.cpp \
    $$PWD/cstarmathpars.cpp

HEADERS += \
    $$PWD/cconversionsmtoooxml.h \
    $$PWD/cstarmathpars.h \
    $$PWD/typeselements.h

