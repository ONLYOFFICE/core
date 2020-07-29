QT -= core
QT -= gui

TARGET = test
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

DEFINES += UNICODE \
          _UNICODE \
           DONT_WRITE_EMBEDDED_FONTS

SOURCES += main.cpp

CORE_ROOT_DIR = $$PWD/../../../core
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

core_windows {
    LIBS += -lAdvapi32
}

# OOXML
ADD_DEPENDENCY(XlsFormatLib, OdfFileWriterLib, OdfFileReaderLib, DocFormatLib, PptFormatLib, RtfFormatLib, RtfFormatLib, TxtXmlFormatLib, ASCOfficeDocxFile2Lib, PPTXFormatLib, DocxFormatLib, CryptoPPLib)
# HTML
ADD_DEPENDENCY(gumbo, HtmlFile2)
# DYNAMIC
ADD_DEPENDENCY(kernel, UnicodeConverter, graphics, HtmlFile)

#BOOST
CONFIG += core_boost_regex
include($$PWD/../../Common/3dParty/boost/boost.pri)

SOURCES += $$CORE_ROOT_DIR/Common/OfficeFileFormatChecker2.cpp

DESTDIR = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX
