#-------------------------------------------------
#
# Project created by QtCreator 2014-10-03T18:30:31
#
#-------------------------------------------------

QT       -= core
QT       -= gui

TARGET = x2t
CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_static_link_libstd
CONFIG += core_x2t
include(../../../Common/base.pri)
include(../../../Common/3dParty/icu/icu.pri)

core_windows {
    QMAKE_LFLAGS += /INCREMENTAL:NO
} else {
    QMAKE_CXXFLAGS += -Wall -Wno-ignored-qualifiers
}

DEFINES += UNICODE \
    _UNICODE \
    #DISABLE_FILE_DOWNLOADER \
    FILTER_FLATE_DECODE_ENABLED \
    DONT_WRITE_EMBEDDED_FONTS \
    AVS_USE_CONVERT_PPTX_TOCUSTOM_VML

DEFINES += PDFFILE_USE_DYNAMIC_LIBRARY
DEFINES += XPS_USE_DYNAMIC_LIBRARY
DEFINES += DJVU_USE_DYNAMIC_LIBRARY
DEFINES += HTMLRENDERER_USE_DYNAMIC_LIBRARY
DEFINES += HTMLFILE_USE_DYNAMIC_LIBRARY
DEFINES += UNICODECONVERTER_USE_DYNAMIC_LIBRARY
DEFINES += FILE_FORMAT_CHECKER_WITH_MACRO

DEFINES += KERNEL_USE_DYNAMIC_LIBRARY
DEFINES += GRAPHICS_USE_DYNAMIC_LIBRARY

core_windows {
	DEFINES += _RWSTD_NO_SETRLIMIT
}

INCLUDEPATH += $$PWD/../../../OOXML
DEPENDPATH += $$PWD/../../../OOXML

INCLUDEPATH += $$PWD/../../../OOXML/PPTXFormat
DEPENDPATH += $$PWD/../../../OOXML/PPTXFormat

INCLUDEPATH += $$PWD/../../../OOXML/Binary/Presentation
DEPENDPATH += $$PWD/../../../OOXML/Binary/Presentation

INCLUDEPATH += $$PWD/../../../OfficeUtils/src
DEPENDPATH += $$PWD/../../../OfficeUtils/src

INCLUDEPATH += $$PWD/../../../OOXML/Binary/Document
DEPENDPATH += $$PWD/../../../OOXML/Binary/Document

##############################################################################################################

SOURCES +=  ../../src/cextracttools.cpp \
            ../../../Common/OfficeFileFormatChecker2.cpp \
            ../../src/ASCConverters.cpp
HEADERS +=  ../../src/cextracttools.h \
            ../../../Common/OfficeFileFormatChecker.h \
            ../../src/ASCConverters.h


#vbaformat
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lVbaFormatLib
#Xls file
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lXlsFormatLib
# odf format
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lOdfFormatLib
#doc file
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lDocFormatLib
# ppt file
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lPptFormatLib
#rtf file
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lRtfFormatLib
#txt(xml) file
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lTxtXmlFormatLib
#bin document
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lBinDocument
#pptxformat
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lPPTXFormatLib
#docxformat
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lDocxFormatLib
#xlsbformat
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lXlsbFormatLib
#cf
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCompoundFileLib

#Crypto++
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib

#All dynamic libs

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, kernel_network, Fb2File, PdfFile, HtmlFile2, EpubFile, XpsFile, DjVuFile, HtmlRenderer, doctrenderer, DocxRenderer)


#####################################################
# внешнее подключение сторонних библиотек

#BOOST
CONFIG += core_boost_regex
include($$PWD/../../../Common/3dParty/boost/boost.pri)

core_windows {
    LIBS += -lAdvapi32
}
########################################################

