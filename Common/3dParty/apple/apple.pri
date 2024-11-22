INCLUDEPATH += $$PWD

# LIBREVENGE
REVENGE_LIB_ROOT = $$PWD/librevenge

INCLUDEPATH += \
	$$REVENGE_LIB_ROOT/inc

HEADERS += $$files($$REVENGE_LIB_ROOT/inc/*.h, true)
HEADERS += $$files($$REVENGE_LIB_ROOT/src/lib/*.h, true)
SOURCES += $$files($$REVENGE_LIB_ROOT/src/lib/*.cpp, true)

# LIBODFGEN
ODF_LIB_ROOT = $$PWD/libodfgen

INCLUDEPATH += \
	$$ODF_LIB_ROOT/inc

HEADERS += $$files($$ODF_LIB_ROOT/inc/libodfgen/*.hxx, true)
HEADERS += $$files($$ODF_LIB_ROOT/src/*.hxx, true)
SOURCES += $$files($$ODF_LIB_ROOT/src/*.cxx, true)

# LIBETONYEK
ETONYEK_LIB_ROOT = $$PWD/libetonyek

INCLUDEPATH += \
	$$ETONYEK_LIB_ROOT/inc \
	$$ETONYEK_LIB_ROOT/src/lib \
	$$ETONYEK_LIB_ROOT/src/lib/contexts \
	$$PWD/mdds/include \
	$$PWD/glm

HEADERS += $$files($$ETONYEK_LIB_ROOT/inc/libetonyek/*.h, true)
HEADERS += $$files($$ETONYEK_LIB_ROOT/src/lib/*.h, true)
SOURCES += $$files($$ETONYEK_LIB_ROOT/src/lib/*.cpp, true)

