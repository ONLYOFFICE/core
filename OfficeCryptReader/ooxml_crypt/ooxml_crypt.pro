TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = ooxml_crypt
CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_static_link_libstd
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib -lCompoundFileLib
ADD_DEPENDENCY(UnicodeConverter, kernel)

include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

CONFIG += open_ssl_common
include($$CORE_ROOT_DIR/Common/3dParty/openssl/openssl.pri)

DEFINES += CRYPTOPP_DISABLE_ASM
DESTDIR = $$CORE_BUILDS_BINARY_PATH

HEADERS += \
    $$PWD/../source/ECMACryptFile.h \
    $$PWD/../source/CryptTransform.h \
    $$PWD/../source/simple_xml_writer.h

SOURCES += \
    $$PWD/../source/ECMACryptFile.cpp \
    $$PWD/../source/CryptTransform.cpp

SOURCES += \
    $$CORE_ROOT_DIR/Common/OfficeFileFormatChecker2.cpp \
    $$CORE_ROOT_DIR/Common/3dParty/pole/pole.cpp \
    $$CORE_ROOT_DIR/MsBinaryFile/DocFile/MemoryStream.cpp \
    $$CORE_ROOT_DIR/OOXML/Base/unicode_util.cpp

SOURCES += \
    $$PWD/main.cpp

core_windows {
    DEFINES -= UNICODE
    DEFINES -= _DEBUG

    LIBS += -lAdvapi32
    LIBS += -lShell32
    LIBS += -lGdi32
    LIBS += -lUser32
    LIBS += -lcrypt32
    LIBS += -lcryptui
    LIBS += -lws2_32
}

core_linux {
    LIBS += -lz -pthread -ldl
    QMAKE_LFLAGS += -Wl,--rpath=./
}
