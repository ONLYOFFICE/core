QT       -= core
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/../../../core
CORE_3DPARTY_DIR = $$CORE_ROOT_DIR/Common/3dParty
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_3DPARTY_DIR/googletest/googletest.pri)

DESTDIR = $$PWD/build

INCLUDEPATH += ../

ADD_DEPENDENCY(doctrenderer)
ADD_DEPENDENCY(HtmlRenderer)
ADD_DEPENDENCY(DocxRenderer)
ADD_DEPENDENCY(PdfFile)
ADD_DEPENDENCY(XpsFile)
ADD_DEPENDENCY(EpubFile)
ADD_DEPENDENCY(DjVuFile)
ADD_DEPENDENCY(Fb2File)
ADD_DEPENDENCY(HtmlFile2)
ADD_DEPENDENCY(graphics)
ADD_DEPENDENCY(kernel)
ADD_DEPENDENCY(kernel_network)
ADD_DEPENDENCY(UnicodeConverter)

core_linux {
    LIBS += -Wl,-unresolved-symbols=ignore-in-shared-libs
    LIBS += -ldl
}

SOURCES += \
    test.cpp

