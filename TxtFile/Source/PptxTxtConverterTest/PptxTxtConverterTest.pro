CONFIG -= qt
QT -= core gui

TARGET   = PptxTxtConverterTest
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
DEFINES += DONT_WRITE_EMBEDDED_FONTS

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)
LIBS += -L$$CORE_BOOST_LIBS

SOURCES += \
    $$CORE_ROOT_DIR/Common/OfficeFileFormatChecker2.cpp \
    $$CORE_ROOT_DIR/X2tConverter/src/cextracttools.cpp \
    $$CORE_ROOT_DIR/X2tConverter/src/ASCConverters.cpp

HEADERS += \
    $$CORE_ROOT_DIR/Common/OfficeFileFormatChecker.h \
    $$CORE_ROOT_DIR/X2tConverter/src/cextracttools.h \
    $$CORE_ROOT_DIR/X2tConverter/src/ASCConverters.h

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lVbaFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lOdfFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lDocFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lPptFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lRtfFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lTxtXmlFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lBinDocument
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lPPTXFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lDocxFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lXlsbFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lXlsFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCompoundFileLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, kernel_network, Fb2File, PdfFile, HtmlFile2, EpubFile, XpsFile, OFDFile, DjVuFile, doctrenderer, DocxRenderer, IWorkFile, HWPFile)

SOURCES += \
    main.cpp
