/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "../../Format/RtfBookmark.cpp"
#include "../../Format/RtfChar.cpp"
#include "../../Format/RtfDocument.cpp"
#include "../../Format/RtfGlobalTables.cpp"
#include "../../Format/RtfOldList.cpp"
#include "../../Format/RtfPicture.cpp"
#include "../../Format/RtfOle.cpp"
#include "../../Format/RtfField.cpp"
#include "../../Format/RtfParagraph.cpp"
#include "../../Format/RtfProperty.cpp"
#include "../../Format/RtfReader.cpp"
#include "../../Format/RtfSection.cpp"
#include "../../Format/RtfShape.cpp"
#include "../../Format/RtfWriter.cpp"
#include "../../Format/RtfMath.cpp"
#include "../../Format/RtfTable.cpp"
#include "../../Format/IdGenerator.cpp"
#include "../../Format/Ole1FormatReader.cpp"
#include "../../Format/RtfLex.cpp"
#include "../../Format/RtfTableCell.cpp"
#include "../../Format/RtfTableRow.cpp"
#include "../../Format/RtfToken.cpp"
#include "../../Format/UniversalConverterUtils.cpp"
#include "../../Format/Utils.cpp"

#include "../../OOXml/Reader/OOXDocumentReader.cpp"
#include "../../OOXml/Reader/OOXTextItemReader.cpp"
#include "../../OOXml/Reader/OOXMathReader.cpp"
#include "../../OOXml/Reader/OOXDrawingGraphicReader.cpp"
#include "../../OOXml/Reader/OOXHeaderReader.cpp"
#include "../../OOXml/Reader/OOXParagraphElementReaders.cpp"
#include "../../OOXml/Reader/OOXReader.cpp"
#include "../../OOXml/Reader/OOXShapeReader.cpp"
#include "../../OOXml/Reader/OOXTableReader.cpp"
#include "../../OOXml/Reader/OOXAbstractNumReader.cpp"
#include "../../OOXml/Reader/OOXAppReader.cpp"
#include "../../OOXml/Reader/OOXBorderReader.cpp"
#include "../../OOXml/Reader/OOXcnfStyleReader.cpp"
#include "../../OOXml/Reader/OOXColorReader.cpp"
#include "../../OOXml/Reader/OOXColorReader2.cpp"
#include "../../OOXml/Reader/OOXColorSchemeReader.cpp"
#include "../../OOXml/Reader/OOXCoreReader.cpp"
#include "../../OOXml/Reader/OOXDocDefaultsReader.cpp"
#include "../../OOXml/Reader/OOXFontReader.cpp"
#include "../../OOXml/Reader/OOXFontSchemeReader.cpp"
#include "../../OOXml/Reader/OOXFontTableReader.cpp"
#include "../../OOXml/Reader/OOXFootnotesReader.cpp"
#include "../../OOXml/Reader/OOXLevelReader.cpp"
#include "../../OOXml/Reader/OOXNumberingMapReader.cpp"
#include "../../OOXml/Reader/OOXNumberingReader.cpp"
#include "../../OOXml/Reader/OOXPictureAnchorReader.cpp"
#include "../../OOXml/Reader/OOXPictureInlineReader.cpp"
#include "../../OOXml/Reader/OOXPictureReader.cpp"
#include "../../OOXml/Reader/OOXpPrTabReader.cpp"
#include "../../OOXml/Reader/OOXShadingReader.cpp"
#include "../../OOXml/Reader/OOXStyleReader.cpp"
#include "../../OOXml/Reader/OOXStyleTableReader.cpp"
#include "../../OOXml/Reader/OOXtblLookReader.cpp"
#include "../../OOXml/Reader/OOXtblpPrReader.cpp"
#include "../../OOXml/Reader/OOXtblPrReader.cpp"
#include "../../OOXml/Reader/OOXtcPrReader.cpp"
#include "../../OOXml/Reader/OOXThemeReader.cpp"

#include "../../OOXml/Writer/OOXDocumentWriter.cpp"
#include "../../OOXml/Writer/OOXWriter.cpp"
#include "../../OOXml/Writer/OOXCommentsWriter.cpp"
#include "../../OOXml/Writer/OOXContentTypesWriter.cpp"
#include "../../OOXml/Writer/OOXFontTableWriter.cpp"
#include "../../OOXml/Writer/OOXFootnoteWriter.cpp"
#include "../../OOXml/Writer/OOXNumberingWriter.cpp"
#include "../../OOXml/Writer/OOXRelsWriter.cpp"
#include "../../OOXml/Writer/OOXSettingsWriter.cpp"
#include "../../OOXml/Writer/OOXStylesWriter.cpp"
#include "../../OOXml/Writer/OOXThemeWriter.cpp"

