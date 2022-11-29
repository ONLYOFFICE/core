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
#include "common_format.cpp"
#include "xlsx_format_logic.cpp"

#include "../../../DocxFormat/Logic/AlternateContent.cpp"
#include "../../../DocxFormat/Logic/Annotations.cpp"
#include "../../../DocxFormat/Logic/Bdo.cpp"
#include "../../../DocxFormat/Logic/Dir.cpp"
#include "../../../DocxFormat/Logic/FldSimple.cpp"
#include "../../../DocxFormat/Logic/Hyperlink.cpp"
#include "../../../DocxFormat/Logic/Paragraph.cpp"
#include "../../../DocxFormat/Logic/ParagraphProperty.cpp"
#include "../../../DocxFormat/Logic/RunProperty.cpp"
#include "../../../DocxFormat/Logic/Run.cpp"
#include "../../../DocxFormat/Logic/Sdt.cpp"
#include "../../../DocxFormat/Logic/SectionProperty.cpp"
#include "../../../DocxFormat/Logic/SmartTag.cpp"
#include "../../../DocxFormat/Logic/Table.cpp"
#include "../../../DocxFormat/Logic/TableProperty.cpp"
#include "../../../DocxFormat/Logic/Vml.cpp"
#include "../../../DocxFormat/Logic/Pict.cpp"
#include "../../../DocxFormat/Logic/FldChar.cpp"
#include "../../../DocxFormat/Logic/RunContent.cpp"
#include "../../../DocxFormat/Logic/VmlOfficeDrawing.cpp"
#include "../../../DocxFormat/Logic/VmlWord.cpp"
#include "../../../DocxFormat/Logic/Shape.cpp"
#include "../../../DocxFormat/Math/oMath.cpp"
#include "../../../DocxFormat/Math/oMathContent.cpp"
#include "../../../DocxFormat/Math/oMathPara.cpp"
#include "../../../DocxFormat/Media/Media.cpp"
#include "../../../DocxFormat/Media/VbaProject.cpp"
#include "../../../DocxFormat/Media/JsaProject.cpp"
#include "../../../DocxFormat/Media/ActiveX.cpp"
#include "../../../DocxFormat/Media/Audio.cpp"
#include "../../../DocxFormat/Media/Video.cpp"
#include "../../../DocxFormat/Media/Image.cpp"
#include "../../../DocxFormat/Media/OleObject.cpp"
#include "../../../DocxFormat/Docx.cpp"
#include "../../../DocxFormat/DocxFlat.cpp"
#include "../../../DocxFormat/Document.cpp"
#include "../../../DocxFormat/Comments.cpp"
#include "../../../DocxFormat/VmlDrawing.cpp"
#include "../../../DocxFormat/Namespaces.cpp"
#include "../../../DocxFormat/App.cpp"
#include "../../../DocxFormat/Core.cpp"
#include "../../../DocxFormat/Bibliography.cpp"
#include "../../../DocxFormat/ContentTypes.cpp"
#include "../../../DocxFormat/CustomXml.cpp"
#include "../../../DocxFormat/File.cpp"
#include "../../../DocxFormat/FileType.cpp"
#include "../../../DocxFormat/Font.cpp"
#include "../../../DocxFormat/FontTable.cpp"
#include "../../../DocxFormat/Footnote.cpp"
#include "../../../DocxFormat/FtnEdn.cpp"
#include "../../../DocxFormat/Settings/Settings.cpp"
#include "../../../DocxFormat/Settings/WebSettings.cpp"
#include "../../../DocxFormat/FileFactory.cpp"
#include "../../../DocxFormat/FileTypes.cpp"
#include "../../../DocxFormat/Numbering.cpp"
#include "../../../DocxFormat/Rels.cpp"
#include "../../../DocxFormat/RId.cpp"
#include "../../../DocxFormat/Styles.cpp"
#include "../../../DocxFormat/UnknowTypeFile.cpp"
#include "../../../DocxFormat/IFileContainer.cpp"
#include "../../../DocxFormat/Document.cpp"
#include "../../../DocxFormat/Drawing/Drawing.cpp"
#include "../../../DocxFormat/Drawing/DrawingExt.cpp"
#include "../../../DocxFormat/Diagram/DiagramColors.cpp"
#include "../../../DocxFormat/Diagram/DiagramData.cpp"
#include "../../../DocxFormat/Diagram/DiagramDrawing.cpp"
#include "../../../DocxFormat/Diagram/DiagramLayout.cpp"
#include "../../../DocxFormat/Diagram/DiagramQuickStyle.cpp"
#include "../../../DocxFormat/External/External.cpp"
#include "../../../DocxFormat/External/ExternalHyperLink.cpp"
#include "../../../Binary/MathEquation/MathEquation.cpp"
#include "../../../DocxFormat/Logic/DocParts.cpp"
#include "../../../DocxFormat/WritingElement.cpp"
#include "../../../DocxFormat/HeaderFooter.cpp"
#include "../../../DocxFormat/Endnote.cpp"
