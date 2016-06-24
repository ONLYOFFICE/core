/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "../../DocDocxConverter/AnnotationReferenceDescriptor.cpp"
#include "../../DocDocxConverter/CharacterPropertiesMapping.cpp"
#include "../../DocDocxConverter/Converter.cpp"
#include "../../DocDocxConverter/DocumentMapping.cpp"
#include "../../DocDocxConverter/DocumentProperties.cpp"
#include "../../DocDocxConverter/FontFamilyName.cpp"
#include "../../DocDocxConverter/FontTableMapping.cpp"
#include "../../DocDocxConverter/FooterMapping.cpp"
#include "../../DocDocxConverter/FormattedDiskPageCHPX.cpp"
#include "../../DocDocxConverter/FormattedDiskPagePAPX.cpp"
#include "../../DocDocxConverter/HeaderAndFooterTable.cpp"
#include "../../DocDocxConverter/HeaderMapping.cpp"
#include "../../DocDocxConverter/LanguageIdMapping.cpp"
#include "../../DocDocxConverter/ListData.cpp"
#include "../../DocDocxConverter/ListLevel.cpp"
#include "../../DocDocxConverter/ListTable.cpp"
#include "../../DocDocxConverter/MainDocumentMapping.cpp"
#include "../../DocDocxConverter/NumberingMapping.cpp"
#include "../../DocDocxConverter/OpenXmlPackage.cpp"
#include "../../DocDocxConverter/ParagraphHeight.cpp"
#include "../../DocDocxConverter/ParagraphPropertiesMapping.cpp"
#include "../../DocDocxConverter/ParagraphPropertyExceptions.cpp"
#include "../../DocDocxConverter/PictureDescriptor.cpp"
#include "../../DocDocxConverter/PieceTable.cpp"
#include "../../DocDocxConverter/PropertiesMapping.cpp"
#include "../../DocDocxConverter/PropertyExceptions.cpp"
#include "../../DocDocxConverter/RevisionData.cpp"
#include "../../DocDocxConverter/SectionPropertiesMapping.cpp"
#include "../../DocDocxConverter/SettingsMapping.cpp"
#include "../../DocDocxConverter/SinglePropertyModifier.cpp"
#include "../../DocDocxConverter/StyleSheet.cpp"
#include "../../DocDocxConverter/StyleSheetDescription.cpp"
#include "../../DocDocxConverter/StyleSheetInformation.cpp"
#include "../../DocDocxConverter/StyleSheetMapping.cpp"
#include "../../DocDocxConverter/TableCellPropertiesMapping.cpp"
#include "../../DocDocxConverter/TableMapping.cpp"
#include "../../DocDocxConverter/TablePropertiesMapping.cpp"
#include "../../DocDocxConverter/TableRowPropertiesMapping.cpp"
#include "../../DocDocxConverter/TextboxMapping.cpp"
#include "../../DocDocxConverter/VMLPictureMapping.cpp"
#include "../../DocDocxConverter/VMLShapeMapping.cpp"
#include "../../DocDocxConverter/VMLShapeTypeMapping.cpp"
#include "../../DocDocxConverter/WordDocument.cpp"
#include "../../DocDocxConverter/WordprocessingDocument.cpp"
#include "../../DocDocxConverter/OfficeDrawing/Record.cpp"
#include "../../DocDocxConverter/OfficeDrawing/RecordFactory.cpp"
#include "../../DocDocxConverter/OfficeDrawing/ShapeTypeFactory.cpp"
