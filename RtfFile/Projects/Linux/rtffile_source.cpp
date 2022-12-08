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

#include "../../OOXml/Writer/OOXDocumentWriter.cpp"
#include "../../OOXml/Writer/OOXWriter.cpp"
