
CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)
ADD_DEPENDENCY(UnicodeConverter, kernel)


SOURCES +=  $$PWD/cconversionsmtoooxml.cpp \
   $$PWD/cooxml2odf.cpp \
    $$PWD/cstarmathpars.cpp

HEADERS += \
    $$PWD/cconversionsmtoooxml.h \
   $$PWD/cooxml2odf.h \
    $$PWD/cstarmathpars.h \
    $$PWD/fontType.h \
    $$PWD/typeConversion.h \
    $$PWD/typeselements.h

