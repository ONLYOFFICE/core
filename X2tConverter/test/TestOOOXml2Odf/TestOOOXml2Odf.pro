QT       -= core
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += core_static_link_libstd

X2T_PATH = $$PWD/../..
CORE_ROOT_DIR = $$PWD/../../../../core
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
include($$CORE_ROOT_DIR/Common/3dParty/googletest/googletest.pri)

DESTDIR = $$PWD/build


INCLUDEPATH += $$CORE_ROOT_DIR/OdfFile
INCLUDEPATH += $$CORE_ROOT_DIR/OdfFile/Common
INCLUDEPATH += $$CORE_ROOT_DIR/Common
INCLUDEPATH += $$CORE_ROOT_DIR/OOXML/PPTXFormat/Logic


LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib 
LIBS += -L$$CORE_BOOST_LIBS

win32 {
LIBS += -lgdi32\
-luser32
}

ADD_DEPENDENCY(kernel)
ADD_DEPENDENCY(kernel_network)
ADD_DEPENDENCY(graphics)

ADD_DEPENDENCY(OdfFormatLib)
ADD_DEPENDENCY(PPTXFormatLib)
ADD_DEPENDENCY(BinDocument)
ADD_DEPENDENCY(DocxFormatLib)
ADD_DEPENDENCY(XlsbFormatLib)
ADD_DEPENDENCY(RtfFormatLib)
ADD_DEPENDENCY(CompoundFileLib)

ADD_DEPENDENCY(DocFormatLib XlsFormatLib PPTFormatLib VbaFormatLib)
ADD_DEPENDENCY(HtmlFile2)
ADD_DEPENDENCY(UnicodeConverter)


core_linux {
    LIBS += -Wl,-unresolved-symbols=ignore-in-shared-libs
    LIBS += -ldl
}

SOURCES += \
    test.cpp \
    $$CORE_ROOT_DIR/Common/OfficeFileFormatChecker2.cpp

DEFINES += DONT_WRITE_EMBEDDED_FONTS
