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

CONFIG += core_x2t
CONFIG += core_boost_date_time

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

SOURCES += \
	../../../Common/OfficeFileFormatChecker2.cpp \
	../../src/cextracttools.cpp \
	../../src/ASCConverters.cpp

HEADERS += \
	../../../Common/OfficeFileFormatChecker.h \
	../../src/cextracttools.h \
	../../src/ASCConverters.h

HEADERS += \
	../../src/lib/common.h \
	\
	../../src/lib/crypt.h \
	\
	../../src/lib/docx.h \
	../../src/lib/pptx.h \
	../../src/lib/xlsx.h \
	\
	../../src/lib/doc.h \
	../../src/lib/rtf.h \
	../../src/lib/txt.h \
	\
	../../src/lib/ppt.h \
	\
	../../src/lib/xls.h \
	../../src/lib/csv.h \
	\
	../../src/lib/html.h \
	\
	../../src/lib/odf.h \
	\
	../../src/lib/pdf_image.h \
	../../src/lib/pdf_oform.h \
	\
	../../src/lib/iwork.h

#vbaformat
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lVbaFormatLib
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
#Xls file
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lXlsFormatLib
#cf
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCompoundFileLib

#Crypto++
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib

#All dynamic libs

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, kernel_network, Fb2File, PdfFile, HtmlFile2, EpubFile, XpsFile, DjVuFile, doctrenderer, DocxRenderer, IWorkFile)


#####################################################
# внешнее подключение сторонних библиотек

#BOOST
CONFIG += core_boost_regex
include($$PWD/../../../Common/3dParty/boost/boost.pri)

core_windows {
    LIBS += -lAdvapi32
}
########################################################

