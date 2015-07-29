#-------------------------------------------------
#
# Project created by QtCreator 2015-04-15T14:10:45
#
#-------------------------------------------------

QT       -= core gui

TARGET = RtfFormatLib
TEMPLATE = lib
CONFIG += staticlib
win32 {
    QMAKE_CXXFLAGS += -std=c++11
} else {
    QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers
}
############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../../SDK/lib

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++:contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_64
}
linux-g++:!contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_32
}
############### destination path ###############

DEFINES += UNICODE _UNICODE _USE_LIBXML2_READER_ _USE_XMLLITE_READER_ USE_LITE_READER

#################### WINDOWS #####################
win32 {
    DEFINES += \
        LIBXML_READER_ENABLED

INCLUDEPATH += ../../../Common/DocxFormat/Source/XML/libxml2/XML/include
INCLUDEPATH += C:/boost_1_58_0
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
#################### LINUX ########################

SOURCES += \
    ../source/DestinationCommand.cpp \
    ../source/RtfBookmark.cpp \
    ../source/RtfChar.cpp \
    ../source/RtfDocument.cpp \
    ../source/RtfGlobalTables.cpp \
    ../source/RtfOldList.cpp \
    ../source/RtfOle.cpp \
    ../source/RtfPicture.cpp \
    ../source/RtfProperty.cpp \
    ../source/RtfReader.cpp \
    ../source/RtfSection.cpp \
    ../source/RtfShape.cpp \
    ../source/RtfWriter.cpp \
    ../source/Reader/OOXHeaderReader.cpp \
    ../source/Reader/OOXParagraphElementReaders.cpp \
    ../source/Reader/OOXReader.cpp \
    ../source/Reader/OOXShapeReader.cpp \
    ../source/Reader/OOXTableReader.cpp \
    ../source/Writer/OOXDocumentWriter.cpp \
    ../source/Writer/OOXWriter.cpp \
    ../source/ConvertationManager.cpp

HEADERS += \
    ../source/Basic.h \
    ../source/ConvertationManager.h \
    ../source/DestinationCommand.h \
    ../source/IdGenerator.h \
    ../source/OOXColorTable.h \
    ../source/RtfBookmark.h \
    ../source/RtfChar.h \
    ../source/RtfDefine.h \
    ../source/RtfDocument.h \
    ../source/RtfErrors.h \
    ../source/RtfField.h \
    ../source/RtfGlobalTables.h \
    ../source/RtfLex.h \
    ../source/RtfMath.h \
    ../source/RtfOle.h \
    ../source/RtfParagraph.h \
    ../source/RtfPicture.h \
    ../source/RtfProperty.h \
    ../source/RtfReader.h \
    ../source/RtfSection.h \
    ../source/RtfShape.h \
    ../source/RtfTable.h \
    ../source/RtfTableCell.h \
    ../source/RtfTableRow.h \
    ../source/RtfToken.h \
    ../source/RtfWriter.h \
    ../source/UniversalConverterUtils.h \
    ../source/Utils.h \
    ../source/Reader/OOXAbstractNumReader.h \
    ../source/Reader/OOXAppReader.h \
    ../source/Reader/OOXBorderReader.h \
    ../source/Reader/OOXcnfStyleReader.h \
    ../source/Reader/OOXColorReader.h \
    ../source/Reader/OOXColorReader2.h \
    ../source/Reader/OOXColorSchemeReader.h \
    ../source/Reader/OOXContentTypeReader.h \
    ../source/Reader/OOXCoreReader.h \
    ../source/Reader/OOXDocDefaultsReader.h \
    ../source/Reader/OOXDocumentReader.h \
    ../source/Reader/OOXFontReader.h \
    ../source/Reader/OOXFontSchemeReader.h \
    ../source/Reader/OOXFontTableReader.h \
    ../source/Reader/OOXFootnotesReader.h \
    ../source/Reader/OOXHeaderReader.h \
    ../source/Reader/OOXLatentStyleReader.h \
    ../source/Reader/OOXLevelReader.h \
    ../source/Reader/OOXMathPropReader.h \
    ../source/Reader/OOXMathReader.h \
    ../source/Reader/OOXNumberingMapReader.h \
    ../source/Reader/OOXNumberingReader.h \
    ../source/Reader/OOXParagraphReader.h \
    ../source/Reader/OOXPictureAnchorReader.h \
    ../source/Reader/OOXPictureGraphicReader.h \
    ../source/Reader/OOXPictureInlineReader.h \
    ../source/Reader/OOXPictureReader.h \
    ../source/Reader/OOXpPrFrameReader.h \
    ../source/Reader/OOXpPrReader.h \
    ../source/Reader/OOXpPrTabReader.h \
    ../source/Reader/OOXReader.h \
    ../source/Reader/OOXReaderBasic.h \
    ../source/Reader/OOXRelsReader.h \
    ../source/Reader/OOXrPrReader.h \
    ../source/Reader/OOXRunReader.h \
    ../source/Reader/OOXSectionPropertyReader.h \
    ../source/Reader/OOXSettingsReader.h \
    ../source/Reader/OOXShadingReader.h \
    ../source/Reader/OOXShapeReader.h \
    ../source/Reader/OOXStyleReader.h \
    ../source/Reader/OOXStyleTableReader.h \
    ../source/Reader/OOXTableCellReader.h \
    ../source/Reader/OOXTableReader.h \
    ../source/Reader/OOXTableRowReader.h \
    ../source/Reader/OOXtblLookReader.h \
    ../source/Reader/OOXtblpPrReader.h \
    ../source/Reader/OOXtblPrReader.h \
    ../source/Reader/OOXtcPrReader.h \
    ../source/Reader/OOXTextItemReader.h \
    ../source/Reader/OOXThemeReader.h \
    ../source/Reader/OOXtrPrReader.h \
    ../source/Writer/OOXContentTypesWriter.h \
    ../source/Writer/OOXDocumentWriter.h \
    ../source/Writer/OOXFontTableWriter.h \
    ../source/Writer/OOXFootnoteWriter.h \
    ../source/Writer/OOXNumberingWriter.h \
    ../source/Writer/OOXRelsWriter.h \
    ../source/Writer/OOXSettingsWriter.h \
    ../source/Writer/OOXStylesWriter.h \
    ../source/Writer/OOXThemeWriter.h \
    ../source/Writer/OOXWriter.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
