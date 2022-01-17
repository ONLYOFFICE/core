QT -= core
QT -= gui

TARGET = PdfWriterTest
TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
include($$CORE_ROOT_DIR/DesktopEditor/graphics/pro/freetype.pri)

ADD_DEPENDENCY(UnicodeConverter, kernel, graphics, PdfWriter)

INCLUDEPATH += \
    $$CORE_ROOT_DIR/DesktopEditor/agg-2.4/include \
    $$CORE_ROOT_DIR/DesktopEditor/cximage/jasper/include \
    $$CORE_ROOT_DIR/DesktopEditor/cximage/jpeg \
    $$CORE_ROOT_DIR/DesktopEditor/cximage/png \
    $$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include \
    $$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include/freetype

core_windows {
DEFINES += NOMINMAX
DEFINES -= UNICODE
DEFINES -= _UNICODE

LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

DEFINES += CRYPTOPP_DISABLE_ASM
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib

SOURCES += PdfWriterTest.cpp
