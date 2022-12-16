QT -= core gui

TARGET = wasm
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

DEFINES += TEST_AS_EXECUTABLE

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

DEFINES += KERNEL_NO_USE_DYNAMIC_LIBRARY
DEFINES += COMMON_OPENSSL_BUILDING_INTERNAL

ADD_DEPENDENCY(UnicodeConverter)

# XML
DEFINES += \
    HAVE_VA_COPY \
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

INCLUDEPATH += \
    $$CORE_ROOT_DIR/DesktopEditor/xml/libxml2/include \
    $$CORE_ROOT_DIR/DesktopEditor/xml/libxml2/include/libxml \
    $$CORE_ROOT_DIR/DesktopEditor/xml/build/qt

SOURCES += \
    $$CORE_ROOT_DIR/DesktopEditor/xml/build/qt/libxml2_all.c \
    $$CORE_ROOT_DIR/DesktopEditor/xml/build/qt/libxml2_all2.c \
    $$CORE_ROOT_DIR/DesktopEditor/xml/src/xmllight.cpp \
    $$CORE_ROOT_DIR/DesktopEditor/xml/src/xmldom.cpp

# KERNEL
SOURCES += \
    $$CORE_ROOT_DIR/DesktopEditor/common/StringBuilder.cpp \
    $$CORE_ROOT_DIR/DesktopEditor/common/StringExt.cpp \
    $$CORE_ROOT_DIR/DesktopEditor/common/Base64.cpp \
    $$CORE_ROOT_DIR/DesktopEditor/common/Path.cpp \
    $$CORE_ROOT_DIR/DesktopEditor/common/File.cpp \
    $$CORE_ROOT_DIR/DesktopEditor/common/Directory.cpp

# ZIP
DEFINES += BUILD_ZLIB_AS_SOURCES

INCLUDEPATH += \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/contrib/minizip \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11 \
    $$CORE_ROOT_DIR/OfficeUtils/src

SOURCES +=  \
    $$CORE_ROOT_DIR/OfficeUtils/src/OfficeUtils.cpp \
    $$CORE_ROOT_DIR/OfficeUtils/src/ZipBuffer.cpp \
    $$CORE_ROOT_DIR/OfficeUtils/src/ZipUtilsCP.cpp \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib_addon.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/contrib/minizip/ioapi.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/contrib/minizip/miniunz.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/contrib/minizip/minizip.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/contrib/minizip/mztools.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/contrib/minizip/unzip.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/contrib/minizip/zip.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/contrib/minizip/ioapibuf.c

core_windows:SOURCES += $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/contrib/minizip/iowin32.c

SOURCES += \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/adler32.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/compress.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/crc32.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/deflate.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/gzclose.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/gzlib.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/gzread.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/gzwrite.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/infback.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/inffast.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/inflate.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/inftrees.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/trees.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/uncompr.c \
    $$CORE_ROOT_DIR/OfficeUtils/src/zlib-1.2.11/zutil.c

# SIGN
SOURCES += \
    $$CORE_ROOT_DIR/DesktopEditor/xmlsec/src/src/XmlTransform.cpp \
    $$CORE_ROOT_DIR/DesktopEditor/xmlsec/src/src/CertificateCommon.cpp \
    $$CORE_ROOT_DIR/DesktopEditor/xmlsec/src/src/OOXMLSigner.cpp \
    $$CORE_ROOT_DIR/DesktopEditor/xmlsec/src/src/OOXMLVerifier.cpp

core_windows {
    LIBS += -lcrypt32
    LIBS += -lcryptui
    LIBS += -lAdvapi32
    LIBS += -lws2_32
    LIBS += -lUser32
    LIBS += -lRpcrt4
    LIBS += -lShell32
}

core_linux {
    LIBS += -ldl
}

DEFINES += SUPPORT_OPENSSL

# OPENSSL
include($$CORE_ROOT_DIR/Common/3dParty/openssl/openssl.pri)

# WASM EXPORT
SOURCES += main.cpp
