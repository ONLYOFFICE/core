QT       -= core
QT       -= gui

VERSION = 1.0.0.3
TARGET = doctrenderer
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += c++11

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include(../../Common/base.pri)

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lUnicodeConverter -lkernel -lgraphics

#CONFIG += build_xp

!build_xp {
    include(../../Common/3dParty/v8/v8.pri)
} else {
    DEFINES += V8_OS_XP
    DESTDIR=$$DESTDIR/xp
    OBJECTS_DIR = $$OBJECTS_DIR/xp
    MOC_DIR     = $$MOC_DIR/xp
    RCC_DIR     = $$RCC_DIR/xp
    UI_DIR      = $$UI_DIR/xp
    include(../../Common/3dParty/v8/v8_xp/v8.pri)
}

SOURCES += \
    memorystream.cpp \
    nativecontrol.cpp \
    doctrenderer.cpp \
    docbuilder.cpp

SOURCES += \
    ../../Common/OfficeFileFormatChecker2.cpp \
    ../../Common/3dParty/pole/pole.cpp \
    ../../Common/DocxFormat/Source/Base/unicode_util.cpp

HEADERS += doctrenderer.h \
    docbuilder.h \
    docbuilder_p.h \
    memorystream.h \
    nativecontrol.h

# downloader
DEFINES += BUIDLER_OPEN_DOWNLOAD_ENABLED
DEFINES += BUIDLER_OPEN_BASE64_ENABLED
