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
#pragma once

namespace TextMark
{
	static const wchar_t ParagraphEnd					= (wchar_t)	13;
	static const wchar_t HardLineBreak					= (wchar_t)	11;
	static const wchar_t BreakingHyphen					= (wchar_t)	4;
	static const wchar_t CellOrRowMark					= (wchar_t)	7;
	static const wchar_t NonRequiredHyphen				= (wchar_t)	31;
	static const wchar_t NonBreakingHyphen				= (wchar_t)	30;
	static const wchar_t NonBreakingSpace				= (wchar_t)	160;
	static const wchar_t Space							= (wchar_t)	32;
	static const wchar_t PageBreakOrSectionMark			= (wchar_t)	12;
	static const wchar_t ColumnBreak					= (wchar_t)	14;
	static const wchar_t Tab							= (wchar_t)	9;
	static const wchar_t FieldBeginMark					= (wchar_t)	19;
	static const wchar_t FieldSeparator					= (wchar_t)	20;
	static const wchar_t FieldEndMark					= (wchar_t)	21;

	//Special WCHARacters (chp.fSpec == 1)

	static const wchar_t CurrentPageNumber				= (wchar_t)	0;
	static const wchar_t Picture						= (wchar_t)	1;
	static const wchar_t AutoNumberedFootnoteReference	= (wchar_t)	2;
	static const wchar_t AutoNumberedEndnoteReference	= (wchar_t)	2;
	static const wchar_t FootnoteSeparator				= (wchar_t)	3;
	static const wchar_t FootnoteContinuation			= (wchar_t)	4;
	static const wchar_t AnnotationReference			= (wchar_t)	5;
	static const wchar_t LineNumber						= (wchar_t)	6;
	static const wchar_t HandAnnotationPicture			= (wchar_t)	7;
	static const wchar_t DrawnObject					= (wchar_t)	8;
	static const wchar_t Symbol							= (wchar_t)	40;
}