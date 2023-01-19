#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       -= core gui

TARGET = RtfFormatLib
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_x2t
include(../../../Common/base.pri)

#BOOST
include($$PWD/../../../Common/3dParty/boost/boost.pri)

DEFINES += UNICODE _UNICODE \
        DONT_WRITE_EMBEDDED_FONTS \
        AVS_USE_CONVERT_PPTX_TOCUSTOM_VML

INCLUDEPATH += ../../../OdfFile/include

core_release {
SOURCES += \
    rtffile_source.cpp
}

core_debug {
SOURCES += \
	../../Format/RtfBookmark.cpp \
	../../Format/RtfChar.cpp \
	../../Format/RtfDocument.cpp \
	../../Format/RtfGlobalTables.cpp \
	../../Format/RtfOldList.cpp \
	../../Format/RtfPicture.cpp \
	../../Format/RtfOle.cpp \
	../../Format/RtfField.cpp \
	../../Format/RtfParagraph.cpp \
	../../Format/RtfProperty.cpp \
	../../Format/RtfReader.cpp \
	../../Format/RtfSection.cpp \
	../../Format/RtfShape.cpp \
	../../Format/RtfWriter.cpp \
	../../Format/RtfMath.cpp \
	../../Format/RtfTable.cpp \
	../../Format/IdGenerator.cpp \
	../../Format/Ole1FormatReader.cpp \
	../../Format/RtfLex.cpp \
	../../Format/RtfTableCell.cpp \
	../../Format/RtfTableRow.cpp \
	../../Format/RtfToken.cpp \
	../../Format/UniversalConverterUtils.cpp \
	../../Format/Utils.cpp \
	\
	../../OOXml/Reader/OOXMathReader.cpp \
	../../OOXml/Reader/OOXDrawingGraphicReader.cpp \
	../../OOXml/Reader/OOXHeaderReader.cpp \
	../../OOXml/Reader/OOXParagraphElementReaders.cpp \
	../../OOXml/Reader/OOXReader.cpp \
	../../OOXml/Reader/OOXShapeReader.cpp \
	../../OOXml/Reader/OOXTableReader.cpp \
	../../OOXml/Reader/OOXDocumentReader.cpp \
	../../OOXml/Reader/OOXTextItemReader.cpp \
	../../OOXml/Reader/OOXAbstractNumReader.cpp \
	../../OOXml/Reader/OOXAppReader.cpp \
	../../OOXml/Reader/OOXBorderReader.cpp \
	../../OOXml/Reader/OOXcnfStyleReader.cpp \
	../../OOXml/Reader/OOXColorReader.cpp \
	../../OOXml/Reader/OOXColorReader2.cpp \
	../../OOXml/Reader/OOXColorSchemeReader.cpp \
	../../OOXml/Reader/OOXCoreReader.cpp \
	../../OOXml/Reader/OOXDocDefaultsReader.cpp \
	../../OOXml/Reader/OOXFontReader.cpp \
	../../OOXml/Reader/OOXFontSchemeReader.cpp \
	../../OOXml/Reader/OOXFontTableReader.cpp \
	../../OOXml/Reader/OOXFootnotesReader.cpp \
	../../OOXml/Reader/OOXLevelReader.cpp \
	../../OOXml/Reader/OOXNumberingMapReader.cpp \
	../../OOXml/Reader/OOXNumberingReader.cpp \
	../../OOXml/Reader/OOXPictureAnchorReader.cpp \
	../../OOXml/Reader/OOXPictureInlineReader.cpp \
	../../OOXml/Reader/OOXPictureReader.cpp \
	../../OOXml/Reader/OOXpPrTabReader.cpp \
	../../OOXml/Reader/OOXShadingReader.cpp \
	../../OOXml/Reader/OOXStyleReader.cpp \
	../../OOXml/Reader/OOXStyleTableReader.cpp \
	../../OOXml/Reader/OOXtblLookReader.cpp \
	../../OOXml/Reader/OOXtblpPrReader.cpp \
	../../OOXml/Reader/OOXtblPrReader.cpp \
	../../OOXml/Reader/OOXtcPrReader.cpp \
	../../OOXml/Reader/OOXThemeReader.cpp \	
	\
	../../OOXml/Writer/OOXDocumentWriter.cpp \
	../../OOXml/Writer/OOXWriter.cpp \
	../../OOXml/Writer/OOXCommentsWriter.cpp \
	../../OOXml/Writer/OOXContentTypesWriter.cpp \
	../../OOXml/Writer/OOXFontTableWriter.cpp \
	../../OOXml/Writer/OOXFootnoteWriter.cpp \
	../../OOXml/Writer/OOXNumberingWriter.cpp \
	../../OOXml/Writer/OOXRelsWriter.cpp \
	../../OOXml/Writer/OOXSettingsWriter.cpp \
	../../OOXml/Writer/OOXStylesWriter.cpp \
	../../OOXml/Writer/OOXThemeWriter.cpp
}

SOURCES += \
	../../Format/DestinationCommand.cpp \
	../../Format/ConvertationManager.cpp

HEADERS += \
	../../Format/Basic.h \
	../../Format/ConvertationManager.h \
	../../Format/DestinationCommand.h \
	../../Format/IdGenerator.h \
	../../Format/RtfBookmark.h \
	../../Format/RtfChar.h \
	../../Format/RtfDefine.h \
	../../Format/RtfDocument.h \
	../../Format/RtfErrors.h \
	../../Format/RtfField.h \
	../../Format/RtfGlobalTables.h \
	../../Format/RtfLex.h \
	../../Format/RtfMath.h \
	../../Format/RtfOle.h \
	../../Format/RtfParagraph.h \
	../../Format/RtfPicture.h \
	../../Format/RtfProperty.h \
	../../Format/RtfReader.h \
	../../Format/RtfSection.h \
	../../Format/RtfShape.h \
	../../Format/RtfTable.h \
	../../Format/RtfTableCell.h \
	../../Format/RtfTableRow.h \
	../../Format/RtfToken.h \
	../../Format/RtfWriter.h \
	../../Format/UniversalConverterUtils.h \
	../../Format/Utils.h \
	../../Format/Ole1FormatReader.h \
	\
	../../OOXml/Reader/OOXAbstractNumReader.h \
	../../OOXml/Reader/OOXAppReader.h \
	../../OOXml/Reader/OOXBorderReader.h \
	../../OOXml/Reader/OOXcnfStyleReader.h \
	../../OOXml/Reader/OOXColorReader.h \
	../../OOXml/Reader/OOXColorReader2.h \
	../../OOXml/Reader/OOXColorSchemeReader.h \
	../../OOXml/Reader/OOXCoreReader.h \
	../../OOXml/Reader/OOXDocDefaultsReader.h \
	../../OOXml/Reader/OOXDocumentReader.h \
	../../OOXml/Reader/OOXFontReader.h \
	../../OOXml/Reader/OOXFontSchemeReader.h \
	../../OOXml/Reader/OOXFontTableReader.h \
	../../OOXml/Reader/OOXFootnotesReader.h \
	../../OOXml/Reader/OOXHeaderReader.h \
	../../OOXml/Reader/OOXLevelReader.h \
	../../OOXml/Reader/OOXMathPropReader.h \
	../../OOXml/Reader/OOXMathReader.h \
	../../OOXml/Reader/OOXNumberingMapReader.h \
	../../OOXml/Reader/OOXNumberingReader.h \
	../../OOXml/Reader/OOXParagraphReader.h \
	../../OOXml/Reader/OOXPictureAnchorReader.h \
	../../OOXml/Reader/OOXPictureGraphicReader.h \
	../../OOXml/Reader/OOXPictureInlineReader.h \
	../../OOXml/Reader/OOXPictureReader.h \
	../../OOXml/Reader/OOXpPrFrameReader.h \
	../../OOXml/Reader/OOXpPrReader.h \
	../../OOXml/Reader/OOXpPrTabReader.h \
	../../OOXml/Reader/OOXReader.h \
	../../OOXml/Reader/OOXReaderBasic.h \
	../../OOXml/Reader/OOXRelsReader.h \
	../../OOXml/Reader/OOXrPrReader.h \
	../../OOXml/Reader/OOXRunReader.h \
	../../OOXml/Reader/OOXSectionPropertyReader.h \
	../../OOXml/Reader/OOXSettingsReader.h \
	../../OOXml/Reader/OOXShadingReader.h \
	../../OOXml/Reader/OOXShapeReader.h \
	../../OOXml/Reader/OOXStyleReader.h \
	../../OOXml/Reader/OOXStyleTableReader.h \
	../../OOXml/Reader/OOXTableCellReader.h \
	../../OOXml/Reader/OOXTableReader.h \
	../../OOXml/Reader/OOXTableRowReader.h \
	../../OOXml/Reader/OOXtblLookReader.h \
	../../OOXml/Reader/OOXtblpPrReader.h \
	../../OOXml/Reader/OOXtblPrReader.h \
	../../OOXml/Reader/OOXtcPrReader.h \
	../../OOXml/Reader/OOXTextItemReader.h \
	../../OOXml/Reader/OOXThemeReader.h \
	../../OOXml/Reader/OOXtrPrReader.h \
	\
	../../OOXml/Writer/OOXContentTypesWriter.h \
	../../OOXml/Writer/OOXDocumentWriter.h \
	../../OOXml/Writer/OOXFontTableWriter.h \
	../../OOXml/Writer/OOXFootnoteWriter.h \
	../../OOXml/Writer/OOXNumberingWriter.h \
	../../OOXml/Writer/OOXRelsWriter.h \
	../../OOXml/Writer/OOXSettingsWriter.h \
	../../OOXml/Writer/OOXStylesWriter.h \
	../../OOXml/Writer/OOXThemeWriter.h \
	../../OOXml/Writer/OOXWriter.h \
	../../OOXml/Writer/OOXCommentsWriter.h
