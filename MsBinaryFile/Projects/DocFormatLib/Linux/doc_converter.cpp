/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "../../../DocFile/AnnotationReferenceDescriptor.cpp"
#include "../../../DocFile/CharacterPropertiesMapping.cpp"
#include "../../../DocFile/Converter.cpp"
#include "../../../DocFile/DocumentMapping.cpp"
#include "../../../DocFile/DocumentProperties.cpp"
#include "../../../DocFile/FontFamilyName.cpp"
#include "../../../DocFile/FontTableMapping.cpp"
#include "../../../DocFile/FooterMapping.cpp"
#include "../../../DocFile/FormattedDiskPageCHPX.cpp"
#include "../../../DocFile/FormattedDiskPagePAPX.cpp"
#include "../../../DocFile/HeaderAndFooterTable.cpp"
#include "../../../DocFile/HeaderMapping.cpp"
#include "../../../DocFile/LanguageIdMapping.cpp"
#include "../../../DocFile/ListData.cpp"
#include "../../../DocFile/ListLevel.cpp"
#include "../../../DocFile/ListTable.cpp"
#include "../../../DocFile/MainDocumentMapping.cpp"
#include "../../../DocFile/NumberingMapping.cpp"
#include "../../../DocFile/OpenXmlPackage.cpp"
#include "../../../DocFile/ParagraphHeight.cpp"
#include "../../../DocFile/ParagraphPropertiesMapping.cpp"
#include "../../../DocFile/ParagraphPropertyExceptions.cpp"
#include "../../../DocFile/PictureDescriptor.cpp"
#include "../../../DocFile/PieceTable.cpp"
#include "../../../DocFile/PropertiesMapping.cpp"
#include "../../../DocFile/PropertyExceptions.cpp"
#include "../../../DocFile/RevisionData.cpp"
#include "../../../DocFile/SectionPropertiesMapping.cpp"
#include "../../../DocFile/SettingsMapping.cpp"
#include "../../../DocFile/SinglePropertyModifier.cpp"
#include "../../../DocFile/StyleSheet.cpp"
#include "../../../DocFile/StyleSheetDescription.cpp"
#include "../../../DocFile/StyleSheetInformation.cpp"
#include "../../../DocFile/StyleSheetMapping.cpp"
#include "../../../DocFile/TableCellPropertiesMapping.cpp"
#include "../../../DocFile/TableMapping.cpp"
#include "../../../DocFile/TablePropertiesMapping.cpp"
#include "../../../DocFile/TableRowPropertiesMapping.cpp"
#include "../../../DocFile/TextboxMapping.cpp"
#include "../../../DocFile/VMLPictureMapping.cpp"
#include "../../../DocFile/VMLShapeMapping.cpp"
#include "../../../DocFile/VMLShapeTypeMapping.cpp"
#include "../../../DocFile/WordDocument.cpp"
#include "../../../DocFile/WordprocessingDocument.cpp"
#include "../../../DocFile/FormFieldData.cpp"
#include "../../../DocFile/AbstractOpenXmlMapping.cpp"
#include "../../../DocFile/AnnotationOwnerList.cpp"
#include "../../../DocFile/AutoSummaryInfo.cpp"
#include "../../../DocFile/BookmarkFirst.cpp"
#include "../../../DocFile/BorderCode.cpp"
#include "../../../DocFile/ByteStructure.cpp"
#include "../../../DocFile/CharacterPropertyExceptions.cpp"
#include "../../../DocFile/CharacterRange.cpp"
#include "../../../DocFile/CommentsMapping.cpp"
#include "../../../DocFile/ConversionContext.cpp"
#include "../../../DocFile/DateAndTime.cpp"
#include "../../../DocFile/DocumentTypographyInfo.cpp"
#include "../../../DocFile/DrawingObjectGrid.cpp"
#include "../../../DocFile/EmuValue.cpp"
#include "../../../DocFile/EndnoteDescriptor.cpp"
#include "../../../DocFile/EndnotesMapping.cpp"
#include "../../../DocFile/FieldCharacter.cpp"
#include "../../../DocFile/FileInformationBlock.cpp"
#include "../../../DocFile/FixedPointNumber.cpp"
#include "../../../DocFile/FootnoteDescriptor.cpp"
#include "../../../DocFile/FootnotesMapping.cpp"
#include "../../../DocFile/FormattedDiskPage.cpp"
#include "../../../DocFile/FormFieldDataMapping.cpp"
#include "../../../DocFile/LanguageId.cpp"
#include "../../../DocFile/LineSpacingDescriptor.cpp"
#include "../../../DocFile/ListFormatOverride.cpp"
#include "../../../DocFile/ListFormatOverrideLevel.cpp"
#include "../../../DocFile/ListFormatOverrideTable.cpp"
#include "../../../DocFile/MemoryStream.cpp"
#include "../../../DocFile/OfficeArtContent.cpp"
#include "../../../DocFile/OleObjectMapping.cpp"
#include "../../../DocFile/PieceDescriptor.cpp"
#include "../../../DocFile/RGBColor.cpp"
#include "../../../DocFile/SectionDescriptor.cpp"
#include "../../../DocFile/SectionPropertyExceptions.cpp"
#include "../../../DocFile/ShadingDescriptor.cpp"
#include "../../../DocFile/SprmTDefTable.cpp"
#include "../../../DocFile/StructuredStorageReader.cpp"
#include "../../../DocFile/TabDescriptor.cpp"
#include "../../../DocFile/TableInfo.cpp"
#include "../../../DocFile/TablePropertyExceptions.cpp"
#include "../../../DocFile/Tbkd.cpp"
#include "../../../DocFile/TwipsValue.cpp"
#include "../../../DocFile/VirtualStreamReader.cpp"
#include "../../../DocFile/WideString.cpp"

#include "../../../DocFile/OfficeDrawing/Record.cpp"
#include "../../../DocFile/OfficeDrawing/RecordFactory.cpp"
#include "../../../DocFile/OfficeDrawing/ShapeTypeFactory.cpp"
#include "../../../DocFile/OfficeDrawing/BitmapBlip.cpp"
#include "../../../DocFile/OfficeDrawing/BlipStoreContainer.cpp"
#include "../../../DocFile/OfficeDrawing/BlipStoreEntry.cpp"
#include "../../../DocFile/OfficeDrawing/ChildAnchor.cpp"
#include "../../../DocFile/OfficeDrawing/ClientAnchor.cpp"
#include "../../../DocFile/OfficeDrawing/ClientData.cpp"
#include "../../../DocFile/OfficeDrawing/DiagramBooleanProperties.cpp"
#include "../../../DocFile/OfficeDrawing/DrawingContainer.cpp"
#include "../../../DocFile/OfficeDrawing/DrawingGroup.cpp"
#include "../../../DocFile/OfficeDrawing/DrawingGroupRecord.cpp"
#include "../../../DocFile/OfficeDrawing/DrawingRecord.cpp"
#include "../../../DocFile/OfficeDrawing/GroupContainer.cpp"
#include "../../../DocFile/OfficeDrawing/GroupShapeRecord.cpp"
#include "../../../DocFile/OfficeDrawing/MetafilePictBlip.cpp"
#include "../../../DocFile/OfficeDrawing/OfficeArtClientTextbox.cpp"
#include "../../../DocFile/OfficeDrawing/OfficeArtFRITContainer.cpp"
#include "../../../DocFile/OfficeDrawing/RegularContainer.cpp"
#include "../../../DocFile/OfficeDrawing/Shape.cpp"
#include "../../../DocFile/OfficeDrawing/ShapeContainer.cpp"
#include "../../../DocFile/OfficeDrawing/ShapeOptions.cpp"
#include "../../../DocFile/OfficeDrawing/ShapeType.cpp"
#include "../../../DocFile/OfficeDrawing/SplitMenuColorContainer.cpp"
#include "../../../DocFile/OfficeDrawing/UnknownRecord.cpp"
