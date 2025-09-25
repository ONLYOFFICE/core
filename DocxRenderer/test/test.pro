CONFIG -= qt
QT -= core gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(UnicodeConverter, kernel, kernel_network, graphics, PdfFile, DjVuFile, XpsFile, DocxRenderer, TextCommandRenderer)

core_linux:include($$PWD/../../Common/3dParty/icu/icu.pri)
core_windows:LIBS += -lgdi32 -ladvapi32 -luser32 -lshell32
core_linux:LIBS += -lz

SOURCES += main.cpp

# for officefilechecker
SOURCES += \
    $$CORE_ROOT_DIR/Common/OfficeFileFormatChecker2.cpp \
    $$CORE_ROOT_DIR/Common/3dParty/pole/pole.cpp \
    $$CORE_ROOT_DIR/OOXML/Base/unicode_util.cpp

DESTDIR = $$PWD_ROOT_DIR/build

#CONFIG += use_test_html_renderer
use_test_html_renderer {
    DEFINES += TEST_FOR_HTML_RENDERER_TEXT

	HEADERS += $$CORE_ROOT_DIR/DesktopEditor/graphics/pro/js/wasm/src/HTMLRendererText.h
	SOURCES += $$CORE_ROOT_DIR/DesktopEditor/graphics/pro/js/wasm/src/HTMLRendererText.cpp
}
