#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       -= core gui

TARGET = DocFormatLib
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_x2t
include(../../../../Common/base.pri)

#BOOST
include($$PWD/../../../../Common/3dParty/boost/boost.pri)

DEFINES +=  UNICODE \
        _UNICODE \
        DONT_WRITE_EMBEDDED_FONTS

SOURCES +=  \
	../../../DocFile/Main/DocFormatLib.cpp \
	../../../DocFile/EncryptionHeader.cpp \
	../../../DocFile/DrawingPrimitives.cpp \
	../../../DocFile/Spa.cpp \
	../../../DocFile/OleObject.cpp \
	../../../Common/Base/XmlTools.cpp

core_release {
SOURCES += \
	doc_converter.cpp
}

core_debug {
SOURCES += \
	../../../DocFile/AnnotationReferenceDescriptor.cpp \
	../../../DocFile/CharacterPropertiesMapping.cpp \
	../../../DocFile/Converter.cpp \
	../../../DocFile/DocumentMapping.cpp \
	../../../DocFile/DocumentProperties.cpp \
	../../../DocFile/FontFamilyName.cpp \
	../../../DocFile/FontTableMapping.cpp \
	../../../DocFile/FooterMapping.cpp \
	../../../DocFile/FormattedDiskPageCHPX.cpp \
	../../../DocFile/FormattedDiskPagePAPX.cpp \
	../../../DocFile/HeaderAndFooterTable.cpp \
	../../../DocFile/HeaderMapping.cpp \
	../../../DocFile/LanguageIdMapping.cpp \
	../../../DocFile/ListData.cpp \
	../../../DocFile/ListLevel.cpp \
	../../../DocFile/ListTable.cpp \
	../../../DocFile/MainDocumentMapping.cpp \
	../../../DocFile/NumberingMapping.cpp \
	../../../DocFile/OpenXmlPackage.cpp \
	../../../DocFile/ParagraphHeight.cpp \
	../../../DocFile/ParagraphPropertiesMapping.cpp \
	../../../DocFile/ParagraphPropertyExceptions.cpp \
	../../../DocFile/PictureDescriptor.cpp \
	../../../DocFile/PieceTable.cpp \
	../../../DocFile/PropertiesMapping.cpp \
	../../../DocFile/PropertyExceptions.cpp \
	../../../DocFile/RevisionData.cpp \
	../../../DocFile/SectionPropertiesMapping.cpp \
	../../../DocFile/SettingsMapping.cpp \
	../../../DocFile/SinglePropertyModifier.cpp \
	../../../DocFile/StyleSheet.cpp \
	../../../DocFile/StyleSheetDescription.cpp \
	../../../DocFile/StyleSheetInformation.cpp \
	../../../DocFile/StyleSheetMapping.cpp \
	../../../DocFile/TableCellPropertiesMapping.cpp \
	../../../DocFile/TableMapping.cpp \
	../../../DocFile/TablePropertiesMapping.cpp \
	../../../DocFile/TableRowPropertiesMapping.cpp \
	../../../DocFile/TextboxMapping.cpp \
	../../../DocFile/VMLPictureMapping.cpp \
	../../../DocFile/VMLShapeMapping.cpp \
	../../../DocFile/VMLShapeTypeMapping.cpp \
	../../../DocFile/WordDocument.cpp \
	../../../DocFile/WordprocessingDocument.cpp \
	../../../DocFile/FormFieldData.cpp \
	../../../DocFile/AbstractOpenXmlMapping.cpp \
	../../../DocFile/AnnotationOwnerList.cpp \
	../../../DocFile/AutoSummaryInfo.cpp \
	../../../DocFile/BookmarkFirst.cpp \
	../../../DocFile/BorderCode.cpp \
	../../../DocFile/ByteStructure.cpp \
	../../../DocFile/CharacterPropertyExceptions.cpp \
	../../../DocFile/CharacterRange.cpp \
	../../../DocFile/CommentsMapping.cpp \
	../../../DocFile/ConversionContext.cpp \
	../../../DocFile/DateAndTime.cpp \
	../../../DocFile/DocumentTypographyInfo.cpp \
	../../../DocFile/DrawingObjectGrid.cpp \
	../../../DocFile/EmuValue.cpp \
	../../../DocFile/EndnoteDescriptor.cpp \
	../../../DocFile/EndnotesMapping.cpp \
	../../../DocFile/FieldCharacter.cpp \
	../../../DocFile/FileInformationBlock.cpp \
	../../../DocFile/FixedPointNumber.cpp \
	../../../DocFile/FootnoteDescriptor.cpp \
	../../../DocFile/FootnotesMapping.cpp \
	../../../DocFile/FormattedDiskPage.cpp \
	../../../DocFile/FormFieldDataMapping.cpp \
	../../../DocFile/LanguageId.cpp \
	../../../DocFile/LineSpacingDescriptor.cpp \
	../../../DocFile/ListFormatOverride.cpp \
	../../../DocFile/ListFormatOverrideLevel.cpp \
	../../../DocFile/ListFormatOverrideTable.cpp \
	../../../DocFile/MemoryStream.cpp \
	../../../DocFile/OfficeArtContent.cpp \
	../../../DocFile/OleObjectMapping.cpp \
	../../../DocFile/PieceDescriptor.cpp \
	../../../DocFile/RGBColor.cpp \
	../../../DocFile/SectionDescriptor.cpp \
	../../../DocFile/SectionPropertyExceptions.cpp \
	../../../DocFile/ShadingDescriptor.cpp \
	../../../DocFile/SprmTDefTable.cpp \
	../../../DocFile/StructuredStorageReader.cpp \
	../../../DocFile/TabDescriptor.cpp \
	../../../DocFile/TableInfo.cpp \
	../../../DocFile/TablePropertyExceptions.cpp \
	../../../DocFile/Tbkd.cpp \
	../../../DocFile/TwipsValue.cpp \
	../../../DocFile/VirtualStreamReader.cpp \
	../../../DocFile/WideString.cpp \
	\
	../../../DocFile/OfficeDrawing/Record.cpp \
	../../../DocFile/OfficeDrawing/RecordFactory.cpp \
	../../../DocFile/OfficeDrawing/ShapeTypeFactory.cpp \
	../../../DocFile/OfficeDrawing/BitmapBlip.cpp \
	../../../DocFile/OfficeDrawing/BlipStoreContainer.cpp \
	../../../DocFile/OfficeDrawing/BlipStoreEntry.cpp \
	../../../DocFile/OfficeDrawing/ChildAnchor.cpp \
	../../../DocFile/OfficeDrawing/ClientAnchor.cpp \
	../../../DocFile/OfficeDrawing/ClientData.cpp \
	../../../DocFile/OfficeDrawing/DiagramBooleanProperties.cpp \
	../../../DocFile/OfficeDrawing/DrawingContainer.cpp \
	../../../DocFile/OfficeDrawing/DrawingGroup.cpp \
	../../../DocFile/OfficeDrawing/DrawingGroupRecord.cpp \
	../../../DocFile/OfficeDrawing/DrawingRecord.cpp \
	../../../DocFile/OfficeDrawing/GroupContainer.cpp \
	../../../DocFile/OfficeDrawing/GroupShapeRecord.cpp \
	../../../DocFile/OfficeDrawing/MetafilePictBlip.cpp \
	../../../DocFile/OfficeDrawing/OfficeArtClientTextbox.cpp \
	../../../DocFile/OfficeDrawing/OfficeArtFRITContainer.cpp \
	../../../DocFile/OfficeDrawing/RegularContainer.cpp \
	../../../DocFile/OfficeDrawing/Shape.cpp \
	../../../DocFile/OfficeDrawing/ShapeContainer.cpp \
	../../../DocFile/OfficeDrawing/ShapeOptions.cpp \
	../../../DocFile/OfficeDrawing/ShapeType.cpp \
	../../../DocFile/OfficeDrawing/SplitMenuColorContainer.cpp \
	../../../DocFile/OfficeDrawing/UnknownRecord.cpp
}

HEADERS +=  \
	../../../DocFile/Main/DocFormatLib.h \
	../../../../Common/3dParty/pole/pole.h \
	../../../Common/Base/Aggregat.h \
	../../../Common/Base/FormatUtils.h \
	../../../Common/Base/IOperand.h \
	../../../Common/Base/IVirtualConstructor.h \
	../../../Common/Base/LanguageCodes.h \
	../../../Common/Base/Singleton.h \
	../../../Common/Base/SPRMCodes.h \
	../../../Common/Base/TargetOptions.h \
	../../../Common/Base/TextMark.h \
	../../../Common/Base/utf8.h \
	../../../Common/Base/XmlTools.h \
	\
	../../../DocFile/AbstractOpenXmlMapping.h \
	../../../DocFile/AnnotationOwnerList.h \
	../../../DocFile/AnnotationReferenceDescriptor.h \
	../../../DocFile/AutoSummaryInfo.h \
	../../../DocFile/BookmarkFirst.h \
	../../../DocFile/BorderCode.h \
	../../../DocFile/ByteStructure.h \
	../../../DocFile/CharacterPropertiesMapping.h \
	../../../DocFile/CharacterPropertyExceptions.h \
	../../../DocFile/CharacterRange.h \
	../../../DocFile/CommentsMapping.h \
	../../../DocFile/ContentTypes.h \
	../../../DocFile/ConversionContext.h \
	../../../DocFile/Converter.h \
	../../../DocFile/DateAndTime.h \
	../../../DocFile/DocumentMapping.h \
	../../../DocFile/DocumentProperties.h \
	../../../DocFile/DocumentTypographyInfo.h \
	../../../DocFile/DrawingObjectGrid.h \
	../../../DocFile/EmuValue.h \
	../../../DocFile/EndnoteDescriptor.h \
	../../../DocFile/EndnotesMapping.h \
	../../../DocFile/FieldCharacter.h \
	../../../DocFile/FileInformationBlock.h \
	../../../DocFile/FixedPointNumber.h \
	../../../DocFile/FontFamilyName.h \
	../../../DocFile/FontTableMapping.h \
	../../../DocFile/FooterMapping.h \
	../../../DocFile/FootnoteDescriptor.h \
	../../../DocFile/FootnotesMapping.h \
	../../../DocFile/FormattedDiskPage.h \
	../../../DocFile/FormattedDiskPageCHPX.h \
	../../../DocFile/FormattedDiskPagePAPX.h \
	../../../DocFile/Global.h \
	../../../DocFile/HeaderAndFooterTable.h \
	../../../DocFile/HeaderMapping.h \
	../../../DocFile/IBinaryReader.h \
	../../../DocFile/IMapping.h \
	../../../DocFile/IVisitable.h \
	../../../DocFile/LanguageId.h \
	../../../DocFile/LanguageIdMapping.h \
	../../../DocFile/LineSpacingDescriptor.h \
	../../../DocFile/ListData.h \
	../../../DocFile/ListFormatOverride.h \
	../../../DocFile/ListFormatOverrideLevel.h \
	../../../DocFile/ListFormatOverrideTable.h \
	../../../DocFile/ListLevel.h \
	../../../DocFile/ListTable.h \
	../../../DocFile/MainDocumentMapping.h \
	../../../DocFile/MemoryStream.h \
	../../../DocFile/NumberingMapping.h \
	../../../DocFile/OfficeArtContent.h \
	../../../DocFile/OleObject.h \
	../../../DocFile/OleObjectMapping.h \
	../../../DocFile/OpenXmlPackage.h \
	../../../DocFile/ParagraphHeight.h \
	../../../DocFile/ParagraphPropertiesMapping.h \
	../../../DocFile/ParagraphPropertyExceptions.h \
	../../../DocFile/PictureDescriptor.h \
	../../../DocFile/PieceDescriptor.h \
	../../../DocFile/PieceTable.h \
	../../../DocFile/Plex.h \
	../../../DocFile/PropertiesMapping.h \
	../../../DocFile/PropertyExceptions.h \
	../../../DocFile/RevisionData.h \
	../../../DocFile/RGBColor.h \
	../../../DocFile/SectionDescriptor.h \
	../../../DocFile/SectionPropertiesMapping.h \
	../../../DocFile/SectionPropertyExceptions.h \
	../../../DocFile/SettingsMapping.h \
	../../../DocFile/ShadingDescriptor.h \
	../../../DocFile/SinglePropertyModifier.h \
	../../../DocFile/Spa.h \
	../../../DocFile/SprmTDefTable.h \
	../../../DocFile/StringTable.h \
	../../../DocFile/StructuredStorageReader.h \
	../../../DocFile/StyleSheet.h \
	../../../DocFile/StyleSheetDescription.h \
	../../../DocFile/StyleSheetInformation.h \
	../../../DocFile/StyleSheetMapping.h \
	../../../DocFile/TabDescriptor.h \
	../../../DocFile/TableCellPropertiesMapping.h \
	../../../DocFile/TableInfo.h \
	../../../DocFile/TableMapping.h \
	../../../DocFile/TablePropertiesMapping.h \
	../../../DocFile/TablePropertyExceptions.h \
	../../../DocFile/TableRowPropertiesMapping.h \
	../../../DocFile/Tbkd.h \
	../../../DocFile/TextboxMapping.h \
	../../../DocFile/TwipsValue.h \
	../../../DocFile/VirtualStreamReader.h \
	../../../DocFile/VMLPictureMapping.h \
	../../../DocFile/VMLShapeMapping.h \
	../../../DocFile/VMLShapeTypeMapping.h \
	../../../DocFile/WideString.h \
	../../../DocFile/WordDocument.h \
	../../../DocFile/WordprocessingDocument.h \
	\
	../../../DocFile/OfficeDrawing/BitmapBlip.h \
	../../../DocFile/OfficeDrawing/BlipStoreContainer.h \
	../../../DocFile/OfficeDrawing/BlipStoreEntry.h \
	../../../DocFile/OfficeDrawing/ChildAnchor.h \
	../../../DocFile/OfficeDrawing/ClientAnchor.h \
	../../../DocFile/OfficeDrawing/ClientData.h \
	../../../DocFile/OfficeDrawing/DrawingContainer.h \
	../../../DocFile/OfficeDrawing/DrawingGroup.h \
	../../../DocFile/OfficeDrawing/DrawingGroupRecord.h \
	../../../DocFile/OfficeDrawing/DrawingRecord.h \
	../../../DocFile/OfficeDrawing/GroupContainer.h \
	../../../DocFile/OfficeDrawing/GroupShapeRecord.h \
	../../../DocFile/OfficeDrawing/MetafilePictBlip.h \
	../../../DocFile/OfficeDrawing/OfficeArtClientTextbox.h \
	../../../DocFile/OfficeDrawing/PathParser.h \
	../../../DocFile/OfficeDrawing/PathSegment.h \
	../../../DocFile/OfficeDrawing/Record.h \
	../../../DocFile/OfficeDrawing/RecordFactory.h \
	../../../DocFile/OfficeDrawing/RegularContainer.h \
	../../../DocFile/OfficeDrawing/Shape.h \
	../../../DocFile/OfficeDrawing/ShapeContainer.h \
	../../../DocFile/OfficeDrawing/ShapeOptions.h \
	../../../DocFile/OfficeDrawing/ShapeType.h \
	../../../DocFile/OfficeDrawing/ShapeTypeFactory.h \
	../../../DocFile/OfficeDrawing/SplitMenuColorContainer.h \
	../../../DocFile/OfficeDrawing/UnknownRecord.h \
	\
	../../../DocFile/OfficeDrawing/Shapetypes/ArcType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/ArrowType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/BevelType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/BlockArcType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/BracketsType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/CalloutsType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/CanType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/Connector.h \
	../../../DocFile/OfficeDrawing/Shapetypes/CubeType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/DiamondType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/DonutType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/FlowChartType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/FoldedCornerlType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/HeartType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/HexagonType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/IsoscelesTriangleType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/LightningBoltType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/LineType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/MoonType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/NoSmokingType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/OctagonType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/OvalType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/ParallelogramType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/PentagonType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/PictureFrameType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/PlaqueType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/PlusType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/RectangleType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/RightTriangleType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/RoundedRectangleType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/SmileyFaceType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/StarType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/SunType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/TextboxType.h \
	../../../DocFile/OfficeDrawing/Shapetypes/TrapezoidType.h \
	../../../DocFile/EncryptionHeader.h \
	../../../DocFile/DrawingPrimitives.h
