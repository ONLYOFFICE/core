CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)
ADD_DEPENDENCY(UnicodeConverter, kernel)

HEADERS += \
	$$PWD/smcustomshapepars.h

SOURCES += \
	$$PWD/smcustomshapepars.cpp


