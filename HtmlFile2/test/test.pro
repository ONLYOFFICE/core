QT -= core
QT -= gui

TARGET = test
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

DEFINES += UNICODE \
          _UNICODE \
           DONT_WRITE_EMBEDDED_FONTS

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

INCLUDEPATH += $$PWD/../../Common/DocxFormat
DEPENDPATH += $$PWD/../../Common/DocxFormat

INCLUDEPATH += $$PWD/../../ASCOfficePPTXFile/
DEPENDPATH += $$PWD/../../ASCOfficePPTXFile/

INCLUDEPATH += $$PWD/../../ASCOfficePPTXFile/Editor/
DEPENDPATH += $$PWD/../../ASCOfficePPTXFile/Editor/

INCLUDEPATH += $$PWD/../../OfficeUtils/src
DEPENDPATH += $$PWD/../../OfficeUtils/src

INCLUDEPATH += $$PWD/../../ASCOfficeDocxFile2
DEPENDPATH += $$PWD/../../ASCOfficeDocxFile2

SOURCES += main.cpp

CORE_ROOT_DIR = $$PWD/../../../core
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lHtmlFile2
#Xls file
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lXlsFormatLib
# odf format writer
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lOdfFileWriterLib
# odf format reader
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lOdfFileReaderLib
#doc file
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lDocFormatLib
# ppt file
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lPptFormatLib
#rtf file
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lRtfFormatLib
#txt(xml) file
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lTxtXmlFormatLib
#docxfile2
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lASCOfficeDocxFile2Lib
#pptxformat
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lPPTXFormatLib
#docxformat
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lDocxFormatLib
#Crypto++
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib

core_windows {
    LIBS += -lAdvapi32
}

ADD_DEPENDENCY(ASCOfficeDocxFile2Lib, PPTXFormatLib, DocxFormatLib, CryptoPPLib)
ADD_DEPENDENCY(kernel, gumbo, UnicodeConverter, graphics, HtmlFile2)
ADD_DEPENDENCY(PdfWriter, PdfReader, HtmlFile, XpsFile, DjVuFile, HtmlRenderer, doctrenderer, Fb2File)

#BOOST
CONFIG += core_boost_regex
include($$PWD/../../Common/3dParty/boost/boost.pri)

DESTDIR = $$PWD/build/$$CORE_BUILDS_PLATFORM_PREFIX
