#-------------------------------------------------
#
# Project created by QtCreator 2014-12-16T18:28:23
#
#-------------------------------------------------

QT       -= core gui

TARGET = DocFormatLib
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

DEFINES +=  UNICODE \
        _UNICODE \
        _USE_XMLLITE_READER_ \
         USE_LITE_READER \
         USE_ATL_CSTRING \
        _USE_LIBXML2_READER_ \
        LIBXML_READER_ENABLED


#################### WINDOWS #####################
win32 {
    DEFINES += \
        LIBXML_READER_ENABLED

INCLUDEPATH += ../../../Common/DocxFormat/Source/XML/libxml2/XML/include
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

SOURCES +=  \
    ../DocFormatLib.cpp \
    ../../Common/MD4/md4c.cpp \
    ../../DocDocxConverter/AnnotationReferenceDescriptor.cpp \
    ../../DocDocxConverter/CharacterPropertiesMapping.cpp \
    ../../DocDocxConverter/Converter.cpp \
    ../../DocDocxConverter/DocumentMapping.cpp \
    ../../DocDocxConverter/DocumentProperties.cpp \
    ../../DocDocxConverter/FontFamilyName.cpp \
    ../../DocDocxConverter/FontTableMapping.cpp \
    ../../DocDocxConverter/FooterMapping.cpp \
    ../../DocDocxConverter/FormattedDiskPageCHPX.cpp \
    ../../DocDocxConverter/FormattedDiskPagePAPX.cpp \
    ../../DocDocxConverter/HeaderAndFooterTable.cpp \
    ../../DocDocxConverter/HeaderMapping.cpp \
    ../../DocDocxConverter/LanguageIdMapping.cpp \
    ../../DocDocxConverter/ListData.cpp \
    ../../DocDocxConverter/ListLevel.cpp \
    ../../DocDocxConverter/ListTable.cpp \
    ../../DocDocxConverter/MainDocumentMapping.cpp \
    ../../DocDocxConverter/NumberingMapping.cpp \
    ../../DocDocxConverter/OpenXmlPackage.cpp \
    ../../DocDocxConverter/ParagraphHeight.cpp \
    ../../DocDocxConverter/ParagraphPropertiesMapping.cpp \
    ../../DocDocxConverter/ParagraphPropertyExceptions.cpp \
    ../../DocDocxConverter/PictureDescriptor.cpp \
    ../../DocDocxConverter/PieceTable.cpp \
    ../../DocDocxConverter/PropertiesMapping.cpp \
    ../../DocDocxConverter/PropertyExceptions.cpp \
    ../../DocDocxConverter/RevisionData.cpp \
    ../../DocDocxConverter/SectionPropertiesMapping.cpp \
    ../../DocDocxConverter/SettingsMapping.cpp \
    ../../DocDocxConverter/SinglePropertyModifier.cpp \
    ../../DocDocxConverter/StyleSheet.cpp \
    ../../DocDocxConverter/StyleSheetDescription.cpp \
    ../../DocDocxConverter/StyleSheetInformation.cpp \
    ../../DocDocxConverter/StyleSheetMapping.cpp \
    ../../DocDocxConverter/TableCellPropertiesMapping.cpp \
    ../../DocDocxConverter/TableMapping.cpp \
    ../../DocDocxConverter/TablePropertiesMapping.cpp \
    ../../DocDocxConverter/TableRowPropertiesMapping.cpp \
    ../../DocDocxConverter/TextboxMapping.cpp \
    ../../DocDocxConverter/VMLPictureMapping.cpp \
    ../../DocDocxConverter/VMLShapeMapping.cpp \
    ../../DocDocxConverter/VMLShapeTypeMapping.cpp \
    ../../DocDocxConverter/WordDocument.cpp \
    ../../DocDocxConverter/WordprocessingDocument.cpp \
    ../../DocDocxConverter/OfficeDrawing/Record.cpp \
    ../../DocDocxConverter/OfficeDrawing/RecordFactory.cpp \
    ../../DocDocxConverter/OfficeDrawing/ShapeTypeFactory.cpp \
    ../../../Common/3dParty/pole/pole.cpp



HEADERS +=  \
    ../DocFormatLib.h \
    ../../Common/Aggregat.h \
    ../../Common/FormatUtils.h \
    ../../Common/IOperand.h \
    ../../Common/IVirtualConstructor.h \
    ../../Common/LanguageCodes.h \
    ../../Common/Singleton.h \
    ../../Common/SPRMCodes.h \
    ../../Common/TargetOptions.h \
    ../../Common/TextMark.h \
    ../../Common/utf8.h \
    ../../Common/XmlTools.h \
    ../../Common/MD4/global.h \
    ../../Common/MD4/md4.h \
    ../../Common/MD4/md4c.h \
    ../../DocDocxConverter/AbstractOpenXmlMapping.h \
    ../../DocDocxConverter/AnnotationOwnerList.h \
    ../../DocDocxConverter/AnnotationReferenceDescriptor.h \
    ../../DocDocxConverter/AutoSummaryInfo.h \
    ../../DocDocxConverter/BookmarkFirst.h \
    ../../DocDocxConverter/BorderCode.h \
    ../../DocDocxConverter/ByteStructure.h \
    ../../DocDocxConverter/CharacterPropertiesMapping.h \
    ../../DocDocxConverter/CharacterPropertyExceptions.h \
    ../../DocDocxConverter/CharacterRange.h \
    ../../DocDocxConverter/CommentsMapping.h \
    ../../DocDocxConverter/ContentTypes.h \
    ../../DocDocxConverter/ConversionContext.h \
    ../../DocDocxConverter/Converter.h \
    ../../DocDocxConverter/DateAndTime.h \
    ../../DocDocxConverter/DocumentMapping.h \
    ../../DocDocxConverter/DocumentProperties.h \
    ../../DocDocxConverter/DocumentTypographyInfo.h \
    ../../DocDocxConverter/DrawingObjectGrid.h \
    ../../DocDocxConverter/EmuValue.h \
    ../../DocDocxConverter/EndnoteDescriptor.h \
    ../../DocDocxConverter/EndnotesMapping.h \
    ../../DocDocxConverter/FieldCharacter.h \
    ../../DocDocxConverter/FileInformationBlock.h \
    ../../DocDocxConverter/FixedPointNumber.h \
    ../../DocDocxConverter/FontFamilyName.h \
    ../../DocDocxConverter/FontTableMapping.h \
    ../../DocDocxConverter/FooterMapping.h \
    ../../DocDocxConverter/FootnoteDescriptor.h \
    ../../DocDocxConverter/FootnotesMapping.h \
    ../../DocDocxConverter/FormattedDiskPage.h \
    ../../DocDocxConverter/FormattedDiskPageCHPX.h \
    ../../DocDocxConverter/FormattedDiskPagePAPX.h \
    ../../DocDocxConverter/Global.h \
    ../../DocDocxConverter/HeaderAndFooterTable.h \
    ../../DocDocxConverter/HeaderMapping.h \
    ../../DocDocxConverter/IBinaryReader.h \
    ../../DocDocxConverter/IMapping.h \
    ../../DocDocxConverter/IVisitable.h \
    ../../DocDocxConverter/LanguageId.h \
    ../../DocDocxConverter/LanguageIdMapping.h \
    ../../DocDocxConverter/LineSpacingDescriptor.h \
    ../../DocDocxConverter/ListData.h \
    ../../DocDocxConverter/ListFormatOverride.h \
    ../../DocDocxConverter/ListFormatOverrideLevel.h \
    ../../DocDocxConverter/ListFormatOverrideTable.h \
    ../../DocDocxConverter/ListLevel.h \
    ../../DocDocxConverter/ListTable.h \
    ../../DocDocxConverter/MainDocumentMapping.h \
    ../../DocDocxConverter/MemoryStream.h \
    ../../DocDocxConverter/NumberingMapping.h \
    ../../DocDocxConverter/OfficeArtContent.h \
    ../../DocDocxConverter/OleObject.h \
    ../../DocDocxConverter/OleObjectMapping.h \
    ../../DocDocxConverter/OpenXmlPackage.h \
    ../../DocDocxConverter/ParagraphHeight.h \
    ../../DocDocxConverter/ParagraphPropertiesMapping.h \
    ../../DocDocxConverter/ParagraphPropertyExceptions.h \
    ../../DocDocxConverter/PictureDescriptor.h \
    ../../DocDocxConverter/PieceDescriptor.h \
    ../../DocDocxConverter/PieceTable.h \
    ../../DocDocxConverter/Plex.h \
    ../../DocDocxConverter/PropertiesMapping.h \
    ../../DocDocxConverter/PropertyExceptions.h \
    ../../DocDocxConverter/RevisionData.h \
    ../../DocDocxConverter/RGBColor.h \
    ../../DocDocxConverter/SectionDescriptor.h \
    ../../DocDocxConverter/SectionPropertiesMapping.h \
    ../../DocDocxConverter/SectionPropertyExceptions.h \
    ../../DocDocxConverter/SettingsMapping.h \
    ../../DocDocxConverter/ShadingDescriptor.h \
    ../../DocDocxConverter/SinglePropertyModifier.h \
    ../../DocDocxConverter/Spa.h \
    ../../DocDocxConverter/SprmTDefTable.h \
    ../../DocDocxConverter/StringTable.h \
    ../../DocDocxConverter/StructuredStorageReader.h \
    ../../DocDocxConverter/StyleSheet.h \
    ../../DocDocxConverter/StyleSheetDescription.h \
    ../../DocDocxConverter/StyleSheetInformation.h \
    ../../DocDocxConverter/StyleSheetMapping.h \
    ../../DocDocxConverter/TabDescriptor.h \
    ../../DocDocxConverter/TableCellPropertiesMapping.h \
    ../../DocDocxConverter/TableInfo.h \
    ../../DocDocxConverter/TableMapping.h \
    ../../DocDocxConverter/TablePropertiesMapping.h \
    ../../DocDocxConverter/TablePropertyExceptions.h \
    ../../DocDocxConverter/TableRowPropertiesMapping.h \
    ../../DocDocxConverter/Tbkd.h \
    ../../DocDocxConverter/TextboxMapping.h \
    ../../DocDocxConverter/TwipsValue.h \
    ../../DocDocxConverter/VirtualStreamReader.h \
    ../../DocDocxConverter/VMLPictureMapping.h \
    ../../DocDocxConverter/VMLShapeMapping.h \
    ../../DocDocxConverter/VMLShapeTypeMapping.h \
    ../../DocDocxConverter/WideString.h \
    ../../DocDocxConverter/WordDocument.h \
    ../../DocDocxConverter/WordprocessingDocument.h \
    ../../DocDocxConverter/OfficeDrawing/BitmapBlip.h \
    ../../DocDocxConverter/OfficeDrawing/BlipStoreContainer.h \
    ../../DocDocxConverter/OfficeDrawing/BlipStoreEntry.h \
    ../../DocDocxConverter/OfficeDrawing/ChildAnchor.h \
    ../../DocDocxConverter/OfficeDrawing/ClientAnchor.h \
    ../../DocDocxConverter/OfficeDrawing/ClientData.h \
    ../../DocDocxConverter/OfficeDrawing/DiagramBooleanProperties.h \
    ../../DocDocxConverter/OfficeDrawing/DrawingContainer.h \
    ../../DocDocxConverter/OfficeDrawing/DrawingGroup.h \
    ../../DocDocxConverter/OfficeDrawing/DrawingGroupRecord.h \
    ../../DocDocxConverter/OfficeDrawing/DrawingRecord.h \
    ../../DocDocxConverter/OfficeDrawing/FillStyleBooleanProperties.h \
    ../../DocDocxConverter/OfficeDrawing/GeometryBooleanProperties.h \
    ../../DocDocxConverter/OfficeDrawing/GeometryTextBooleanProperties.h \
    ../../DocDocxConverter/OfficeDrawing/GroupContainer.h \
    ../../DocDocxConverter/OfficeDrawing/GroupShapeBooleanProperties.h \
    ../../DocDocxConverter/OfficeDrawing/GroupShapeRecord.h \
    ../../DocDocxConverter/OfficeDrawing/LineStyleBooleanProperties.h \
    ../../DocDocxConverter/OfficeDrawing/MetafilePictBlip.h \
    ../../DocDocxConverter/OfficeDrawing/OfficeArtClientTextbox.h \
    ../../DocDocxConverter/OfficeDrawing/PathParser.h \
    ../../DocDocxConverter/OfficeDrawing/PathSegment.h \
    ../../DocDocxConverter/OfficeDrawing/ProtectionBooleanProperties.h \
    ../../DocDocxConverter/OfficeDrawing/Record.h \
    ../../DocDocxConverter/OfficeDrawing/RecordFactory.h \
    ../../DocDocxConverter/OfficeDrawing/RegularContainer.h \
    ../../DocDocxConverter/OfficeDrawing/ShadowStyleBooleanProperties.h \
    ../../DocDocxConverter/OfficeDrawing/Shape.h \
    ../../DocDocxConverter/OfficeDrawing/ShapeContainer.h \
    ../../DocDocxConverter/OfficeDrawing/ShapeOptions.h \
    ../../DocDocxConverter/OfficeDrawing/ShapeType.h \
    ../../DocDocxConverter/OfficeDrawing/ShapeTypeFactory.h \
    ../../DocDocxConverter/OfficeDrawing/SplitMenuColorContainer.h \
    ../../DocDocxConverter/OfficeDrawing/UnknownRecord.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/ArcType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/ArrowType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/BevelType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/BlockArcType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/BracketsType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/CalloutsType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/CanType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/Connector.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/CubeType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/DiamondType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/DonutType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/FlowChartType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/FoldedCornerlType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/HeartType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/HexagonType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/IsoscelesTriangleType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/LightningBoltType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/LineType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/MoonType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/NoSmokingType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/OctagonType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/OvalType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/ParallelogramType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/PentagonType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/PictureFrameType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/PlaqueType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/PlusType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/RectangleType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/RightTriangleType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/RoundedRectangleType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/SmileyFaceType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/StarType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/SunType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/TextboxType.h \
    ../../DocDocxConverter/OfficeDrawing/Shapetypes/TrapezoidType.h \
    ../../../Common/3dParty/pole/pole.h



unix {
    target.path = /usr/lib
    INSTALLS += target
}
