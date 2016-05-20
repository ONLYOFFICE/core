#-------------------------------------------------
#
# Project created by QtCreator 2014-10-03T18:30:31
#
#-------------------------------------------------

QT       -= core
QT       -= gui

VERSION = 2.0.2.367
DEFINES += INTVER=$$VERSION

mac {
    message(64 bit)
    TARGET = x2t
} else {
contains(QMAKE_HOST.arch, x86_64):{
#64bit
message(64 bit)
    TARGET = x2t
}
!contains(QMAKE_HOST.arch, x86_64):{
#32bit
message(32 bit)
    TARGET = x2t32
}
}

TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

win32 {
    QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
    QMAKE_LFLAGS += /INCREMENTAL:NO

    contains(QMAKE_TARGET.arch, x86_64):{
        QMAKE_LFLAGS_CONSOLE  = /SUBSYSTEM:CONSOLE,5.02
    } else {
        QMAKE_LFLAGS_CONSOLE  = /SUBSYSTEM:CONSOLE,5.01
    }

} else {
    QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers
}

CONFIG += c++11

#CONFIG += static_link_libstd
static_link_libstd {
    QMAKE_LFLAGS += -static-libstdc++ -static-libgcc
}

#CONFIG += build_for_centos6
build_for_centos6 {
    QMAKE_LFLAGS += -Wl,--dynamic-linker=./ld-linux-x86-64.so.2
}

DEFINES += UNICODE \
    _UNICODE \
    _USE_LIBXML2_READER_ \
    _USE_XMLLITE_READER_ \
    USE_LITE_READER \
    PPTX_DEF\
    PPT_DEF\
    ENABLE_PPT_TO_PPTX_CONVERT\
    NODOCX \
    #DISABLE_FILE_DOWNLOADER \
    FILTER_FLATE_DECODE_ENABLED \
    CXIMAGE_DONT_DECLARE_TCHAR \
    BUILD_CONFIG_FULL_VERSION \
    DONT_WRITE_EMBEDDED_FONTS \
    AVS_USE_CONVERT_PPTX_TOCUSTOM_VML

DEFINES += PDFREADER_USE_DYNAMIC_LIBRARY
DEFINES += PDFWRITER_USE_DYNAMIC_LIBRARY
DEFINES += XPS_USE_DYNAMIC_LIBRARY
DEFINES += DJVU_USE_DYNAMIC_LIBRARY
DEFINES += HTMLRENDERER_USE_DYNAMIC_LIBRARY
DEFINES += HTMLFILE_USE_DYNAMIC_LIBRARY
DEFINES += UNICODECONVERTER_USE_DYNAMIC_LIBRARY

#################### WINDOWS #####################
win32 {
    DEFINES += \
        LIBXML_READER_ENABLED

INCLUDEPATH += ../../../Common/DocxFormat/Source/XML/libxml2/XML/include\
INCLUDEPATH += ../../../OfficeUtils/src/zlib-1.2.3
}
#################### WINDOWS #####################

#################### LINUX ########################
linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
        LINUX \
        _LINUX \
        _LINUX_QT

INCLUDEPATH += /usr/include/libxml2
}

mac {
    DEFINES += \
        LINUX \
        _LINUX \
        _LINUX_QT \
        _MAC \
        MAC \
        LIBXML_READER_ENABLED

INCLUDEPATH += ../../../Common/DocxFormat/Source/XML/libxml2/XML/include\
INCLUDEPATH += ../../../OfficeUtils/src/zlib-1.2.3
}
#################### LINUX ########################

INCLUDEPATH += \
    ../../../DesktopEditor/freetype-2.5.2/include

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

SOURCES += ../../src/main.cpp \
	../../src/cextracttools.cpp \
    ../../../Common/OfficeFileFormatChecker2.cpp \
	../../src/ASCConverters.cpp
HEADERS += ../../src/cextracttools.h \
    ../../../Common/OfficeFileFormatChecker.h \
	../../src/ASCConverters.h

###############################################################################################################

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../../build/lib
DESTINATION_BIN_PATH = $$PWD/../../../build/bin

win32:contains(QMAKE_TARGET.arch, x86_64):{
    message(win64)
	DESTINATION_BIN_PATH = $$DESTINATION_BIN_PATH/windows
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
    message(win32)
	DESTINATION_BIN_PATH = $$DESTINATION_BIN_PATH/windows
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++ | linux-g++-64 | linux-g++-32:contains(QMAKE_HOST.arch, x86_64):{
    message(linuX)
    
    LIBS += $$DESTINATION_BIN_PATH/icu/linux_64/libicuuc.so.55
    LIBS += $$DESTINATION_BIN_PATH/icu/linux_64/libicudata.so.55
    
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/linux_64
	DESTINATION_BIN_PATH = $$DESTINATION_BIN_PATH/linux


}
linux-g++ | linux-g++-64 | linux-g++-32:!contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/linux_32
	DESTINATION_BIN_PATH = $$DESTINATION_BIN_PATH/linux
}

mac {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/mac_64
	DESTINATION_BIN_PATH = $$DESTINATION_BIN_PATH/mac
}
DESTDIR = $$DESTINATION_BIN_PATH
############### destination path ###############

message(destination sdk path = $$DESTINATION_SDK_PATH)

CONFIG(debug, debug|release) {
    message(Debug)
    BOOST_LIBNAME_POSTFIX = -gd
} else {
    message(Release)
}
#Xls file
LIBS += -L$$DESTINATION_SDK_PATH -lXlsFormatLib
# odf format writer
LIBS += -L$$DESTINATION_SDK_PATH -lOdfFileWriterLib
# odf format reader
LIBS += -L$$DESTINATION_SDK_PATH -lOdfFileReaderLib
#doc file
LIBS += -L$$DESTINATION_SDK_PATH -lDocFormatLib
# ppt file
LIBS += -L$$DESTINATION_SDK_PATH -lPptFormatLib
#rtf file
LIBS += -L$$DESTINATION_SDK_PATH -lRtfFormatLib
#txt(xml) file
LIBS += -L$$DESTINATION_SDK_PATH -lTxtXmlFormatLib
# pdf writer
LIBS += -L$$DESTINATION_SDK_PATH -lPdfWriter
#docxfile2
LIBS += -L$$DESTINATION_SDK_PATH -lASCOfficeDocxFile2Lib
#pptxformat
LIBS += -L$$DESTINATION_SDK_PATH -lPPTXFormatLib
#docxformat
LIBS += -L$$DESTINATION_SDK_PATH -lDocxFormatLib
#office utils
LIBS += -L$$DESTINATION_SDK_PATH -lOfficeUtils
#graphics
LIBS += -L$$DESTINATION_SDK_PATH -lgraphics
#doctrenderer
LIBS += -L$$DESTINATION_SDK_PATH -ldoctrenderer
#HtmlRenderer
LIBS += -L$$DESTINATION_SDK_PATH -lHtmlRenderer
LIBS += -L$$DESTINATION_SDK_PATH -lPdfReader
LIBS += -L$$DESTINATION_SDK_PATH -lDjVuFile
LIBS += -L$$DESTINATION_SDK_PATH -lXpsFile
#HtmlFile
LIBS += -L$$DESTINATION_SDK_PATH -lHtmlFile
#UnicodeConverter
LIBS += -L$$DESTINATION_SDK_PATH -lUnicodeConverter


#####################################################
# внешнее подключение сторонних библиотек
win32 {
    LIBS += -lurlmon
    LIBS += -LC:\boost_1_58_0\stage\lib -llibboost_regex-vc120-mt$$BOOST_LIBNAME_POSTFIX-1_58
}

linux-g++ | linux-g++-64 | linux-g++-32 {
    LIBS += -lboost_regex
    LIBS += -lz
    LIBS += -lxml2
    LIBS += -lcurl
}

mac {
    LIBS += -L$$PWD/../../../Common/boost_1_58_0/stage/lib -lboost_regex
    LIBS += -L$$DESTINATION_SDK_PATH -llibxml

    LIBS += -framework AppKit
}
########################################################

