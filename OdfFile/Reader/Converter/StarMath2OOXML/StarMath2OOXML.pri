
CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
ADD_DEPENDENCY(UnicodeConverter, kernel)

SOURCES +=  $$PWD/cconversionsmtoooxml.cpp \
    $$PWD/cstarmathpars.cpp

HEADERS += \
    $$PWD/cconversionsmtoooxml.h \
    $$PWD/cstarmathpars.h \
    $$PWD/typeConversion.h \
    $$PWD/typeselements.h

