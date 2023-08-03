QT       -= core
QT       -= gui

VERSION = 1.0.0.1
TARGET = osign
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

DEFINES += OSIGN_BUILDING

HEADERS += \
    include/osign.h

HEADERS += \
    src/utils.h

SOURCES += \
    src/buffer.cpp \
	src/storage.cpp \
	src/certificate.cpp

core_windows {
    LIBS += -lcrypt32
	LIBS += -lcryptui
	LIBS += -lAdvapi32
	LIBS += -lws2_32
	LIBS += -lUser32
}

DEFINES += SUPPORT_OPENSSL
HEADERS += src/Certificate_openssl.h

support_oform {
    DEFINES += SUPPORT_OFORM
	HEADERS += src/Certificate_oform.h
}

CONFIG += open_ssl_common
include($$CORE_ROOT_DIR/Common/3dParty/openssl/openssl.pri)

# KERNEL ---
DEFINES += KERNEL_USE_DYNAMIC_LIBRARY_BUILDING
# BASE64
HEADERS += $$CORE_ROOT_DIR/DesktopEditor/common/Base64.h
SOURCES += $$CORE_ROOT_DIR/DesktopEditor/common/Base64.cpp

# FILE
HEADERS += $$CORE_ROOT_DIR/DesktopEditor/common/File.h
SOURCES += $$CORE_ROOT_DIR/DesktopEditor/common/File.cpp

core_ios {
    OBJECTIVE_SOURCES += $$CORE_ROOT_DIR/DesktopEditor/common/File_ios.mm
    LIBS += -framework Foundation
}
# ----------

build_xp:DESTDIR=$$DESTDIR/xp
