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
#include "common_formatlib.cpp"
#include "xlsxformatlib_logic.cpp"

#include "../Source/DocxFormat/Logic/AlternateContent.cpp"
#include "../Source/DocxFormat/Logic/Annotations.cpp"
#include "../Source/DocxFormat/Logic/Bdo.cpp"
#include "../Source/DocxFormat/Logic/Dir.cpp"
#include "../Source/DocxFormat/Logic/FldSimple.cpp"
#include "../Source/DocxFormat/Logic/Hyperlink.cpp"
#include "../Source/DocxFormat/Logic/Paragraph.cpp"
#include "../Source/DocxFormat/Logic/ParagraphProperty.cpp"
#include "../Source/DocxFormat/Logic/RunProperty.cpp"
#include "../Source/DocxFormat/Logic/Sdt.cpp"
#include "../Source/DocxFormat/Logic/SectionProperty.cpp"
#include "../Source/DocxFormat/Logic/SmartTag.cpp"
#include "../Source/DocxFormat/Logic/Table.cpp"
#include "../Source/DocxFormat/Logic/TableProperty.cpp"
#include "../Source/DocxFormat/Logic/Vml.cpp"
#include "../Source/DocxFormat/Math/oMath.cpp"
#include "../Source/DocxFormat/Math/oMathContent.cpp"
#include "../Source/DocxFormat/Math/oMathPara.cpp"
#include "../Source/DocxFormat/Media/VbaProject.cpp"
#include "../Source/DocxFormat/Media/JsaProject.cpp"
#include "../Source/DocxFormat/Media/ActiveX.cpp"
#include "../Source/DocxFormat/Docx.cpp"
#include "../Source/DocxFormat/Comments.cpp"
#include "../Source/DocxFormat/VmlDrawing.cpp"
#include "../Source/DocxFormat/App.cpp"
#include "../Source/DocxFormat/Core.cpp"
#include "../Source/DocxFormat/FileFactory.cpp"
#include "../Source/DocxFormat/IFileContainer.cpp"
#include "../Source/DocxFormat/Drawing/DrawingExt.cpp"
#include "../Source/MathEquation/MathEquation.cpp"

