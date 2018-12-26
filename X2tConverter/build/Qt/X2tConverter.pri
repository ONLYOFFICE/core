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

    contains(QMAKE_TARGET.arch, x86_64):{
        QMAKE_LFLAGS_CONSOLE  = /SUBSYSTEM:CONSOLE,5.02
    } else {
        QMAKE_LFLAGS_CONSOLE  = /SUBSYSTEM:CONSOLE,5.01
    }

} else {
    QMAKE_CXXFLAGS += -Wall -Wno-ignored-qualifiers
}

core_linux {
    QMAKE_LFLAGS += -Wl,--rpath=./:./system
}

DEFINES += UNICODE \
    _UNICODE \
    #DISABLE_FILE_DOWNLOADER \
    FILTER_FLATE_DECODE_ENABLED \
    DONT_WRITE_EMBEDDED_FONTS \
    AVS_USE_CONVERT_PPTX_TOCUSTOM_VML

DEFINES += PDFREADER_USE_DYNAMIC_LIBRARY
DEFINES += PDFWRITER_USE_DYNAMIC_LIBRARY
DEFINES += XPS_USE_DYNAMIC_LIBRARY
DEFINES += DJVU_USE_DYNAMIC_LIBRARY
DEFINES += HTMLRENDERER_USE_DYNAMIC_LIBRARY
DEFINES += HTMLFILE_USE_DYNAMIC_LIBRARY
DEFINES += UNICODECONVERTER_USE_DYNAMIC_LIBRARY
DEFINES += FILE_FORMAT_CHECKER_WITH_MACRO

DEFINES += KERNEL_USE_DYNAMIC_LIBRARY
DEFINES += GRAPHICS_USE_DYNAMIC_LIBRARY
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lkernel -lgraphics

INCLUDEPATH += $$PWD/../../../Common/DocxFormat
DEPENDPATH += $$PWD/../../../Common/DocxFormat

INCLUDEPATH += $$PWD/../../../ASCOfficePPTXFile/
DEPENDPATH += $$PWD/../../../ASCOfficePPTXFile/

INCLUDEPATH += $$PWD/../../../ASCOfficePPTXFile/Editor/
DEPENDPATH += $$PWD/../../../ASCOfficePPTXFile/Editor/

INCLUDEPATH += $$PWD/../../../OfficeUtils/src
DEPENDPATH += $$PWD/../../../OfficeUtils/src

INCLUDEPATH += $$PWD/../../../ASCOfficeDocxFile2
DEPENDPATH += $$PWD/../../../ASCOfficeDocxFile2

##############################################################################################################

SOURCES +=  ../../src/cextracttools.cpp \
            ../../../Common/OfficeFileFormatChecker2.cpp \
            ../../src/ASCConverters.cpp
HEADERS +=  ../../src/cextracttools.h \
            ../../../Common/OfficeFileFormatChecker.h \
            ../../src/ASCConverters.h


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
# pdf writer
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lPdfWriter
#docxfile2
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lASCOfficeDocxFile2Lib
#pptxformat
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lPPTXFormatLib
#docxformat
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lDocxFormatLib
#doctrenderer
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -ldoctrenderer
#HtmlRenderer
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lHtmlRenderer
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lPdfReader
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lDjVuFile
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lXpsFile
#HtmlFile
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lHtmlFile
#UnicodeConverter
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lUnicodeConverter
#Crypto++
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib

#####################################################
# внешнее подключение сторонних библиотек

#BOOST
CONFIG += core_boost_regex
include($$PWD/../../../Common/3dParty/boost/boost.pri)

core_windows {
    LIBS += -lAdvapi32
}
########################################################

