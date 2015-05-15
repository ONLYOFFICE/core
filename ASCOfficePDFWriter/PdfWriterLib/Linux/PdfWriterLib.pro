#-------------------------------------------------
#
# Project created by QtCreator 2015-02-17T12:38:00
#
#-------------------------------------------------

QT       -= core gui

TARGET =  PdfWriterLib
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers

DEFINES += UNICODE \
           _UNICODE \
            LINUX \
           _LINUX_QT \
           _USE_LIBXML2_READER_ \
           _USE_XMLLITE_READER_ \
            USE_LITE_READER \
            BUILD_CONFIG_FULL_VERSION \
            FILTER_FLATE_DECODE_ENABLED \
            DONT_WRITE_EMBEDDED_FONTS

INCLUDEPATH += \
    ../../../DesktopEditor/agg-2.4/include \
    ../../../DesktopEditor/freetype-2.5.2/include \
    ../../../DesktopEditor/cximage/zlib

LIBS += -lxml2

INCLUDEPATH += \
    /usr/include/libxml2

SOURCES += \
    ../PdfWriterRenderer.cpp \
    ../ToUnicode.cpp \
    ../Encoder.cpp \
    ../FontTrueType.cpp \
    ../Objects.cpp \
    ../Encrypt.cpp \
    ../Image.cpp \
    ../GraphicState.cpp \
    ../Annotation.cpp \
    ../Font.cpp \
    ../PdfWriterLib.cpp \
    ../Common.cpp \
    ../PageOperations.cpp \
    ../Outline.cpp \
    ../List.cpp \
    ../EncryptDictionary.cpp \
    ../Pages.cpp \
    ../FontTypes.cpp \
    ../Destination.cpp \
    ../Document.cpp \
    ../MemoryManager.cpp \
    ../FontBase.cpp \
    ../FontCID.cpp \
    ../MetaData.cpp \
    ../Streams.cpp \
    ../Catalog.cpp \
    ../PageLabel.cpp \
    ../Utils.cpp \
    ../OnlineOfficeBinToPdf.cpp \
    ../../../Common/DocxFormat/Source/XML/stringcommon.cpp \
    ../../../Common/DocxFormat/Source/SystemUtility/FileSystem/DirectoryPosix.cpp \
    ../../../Common/3dParty/pole/pole.cpp \
    ../../../Common/DocxFormat/Source/XML/libxml2/libxml2.cpp \
    ../../../Common/DocxFormat/Source/Base/unicode_util.cpp \
    ../../../ASCOfficePPTXFile/Editor/FontPicker.cpp \
    ../../../DesktopEditor/fontengine/ApplicationFonts.cpp \
    ../../../DesktopEditor/fontengine/FontManager.cpp

HEADERS += \
    ../Catalog.h \
    ../FontTrueTypeWriter.h \
    ../Encoder.h \
    ../Document.h \
    ../Pattern.h \
    ../Encrypt.h \
    ../Image.h \
    ../GraphicState.h \
    ../MetaData.h \
    ../Streams.h \
    ../Annotation.h \
    ../Errors.h \
    ../Font.h \
    ../Utils.h \
    ../ToUnicode.h \
    ../Common.h \
    ../FontType1.h \
    ../PageOperations.h \
    ../EncryptDictionary.h \
    ../FontBase14.h \
    ../ImageExt.h \
    ../FontCID.h \
    ../PageLabel.h \
    ../PDF.h \
    ../PdfWriterLib.h \
    ../FontTypes.h \
    ../FontTrueType.h \
    ../Pages.h \
    ../Objects.h \
    ../LZWEncoder.h \
    ../Destination.h \
    ../Outline.h \
    ../List.h \
    ../MemoryManager.h \
    ../FontBase.h \
    ../OnlineOfficeBinToPdf.h \
    ../EncoderConstants.h \
    ../Consts.h \
    ../Types.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
