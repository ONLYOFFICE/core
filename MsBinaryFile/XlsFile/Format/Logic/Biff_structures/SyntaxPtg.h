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

#include "Ptg.h"

namespace XLS
{

class SyntaxPtg
{
public:
	static const bool is_operators(std::wstring::const_iterator first, std::wstring::const_iterator last);
	static const bool extract_PtgAdd(std::wstring::const_iterator& first, std::wstring::const_iterator last, const bool operand_expected);
	static const bool extract_PtgSub(std::wstring::const_iterator& first, std::wstring::const_iterator last, const bool operand_expected);
	static const bool extract_PtgPercent(std::wstring::const_iterator& first, std::wstring::const_iterator last, const bool operand_expected);
	static const bool extract_PtgMul(std::wstring::const_iterator& first, std::wstring::const_iterator last);
	static const bool extract_PtgDiv(std::wstring::const_iterator& first, std::wstring::const_iterator last);
	static const bool extract_PtgPower(std::wstring::const_iterator& first, std::wstring::const_iterator last);

	static const bool extract_PtgEq(std::wstring::const_iterator& first, std::wstring::const_iterator last);
	static const bool extract_PtgNe(std::wstring::const_iterator& first, std::wstring::const_iterator last);
	static const bool extract_PtgLe(std::wstring::const_iterator& first, std::wstring::const_iterator last);
	static const bool extract_PtgLt(std::wstring::const_iterator& first, std::wstring::const_iterator last);
	static const bool extract_PtgGe(std::wstring::const_iterator& first, std::wstring::const_iterator last);
	static const bool extract_PtgGt(std::wstring::const_iterator& first, std::wstring::const_iterator last);

	static const bool extract_PtgConcat(std::wstring::const_iterator& first, std::wstring::const_iterator last);
	static const bool extract_PtgUnion(std::wstring::const_iterator& first, std::wstring::const_iterator last);
	static const bool is_PtgIsect(std::wstring::const_iterator& first, std::wstring::const_iterator last);
	static const bool extract_PtgIsect(std::wstring::const_iterator& first, std::wstring::const_iterator last);
	static const bool extract_PtgRange(std::wstring::const_iterator& first, std::wstring::const_iterator last);
	
	static const bool extract_comma(std::wstring::const_iterator& first, std::wstring::const_iterator last);
	static const bool extract_semicolon(std::wstring::const_iterator& first, std::wstring::const_iterator last);

	static const bool extract_PtgUplus(std::wstring::const_iterator& first, std::wstring::const_iterator last, const bool operand_expected);
	static const bool extract_PtgUminus(std::wstring::const_iterator& first, std::wstring::const_iterator last, const bool operand_expected);

	static const bool extract_PtgInt(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str);
	static const bool extract_PtgNum(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str);
	static const bool extract_PtgBool(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str);
	static const bool extract_PtgStr(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str);
	static const bool extract_PtgName(std::wstring::const_iterator& first, std::wstring::const_iterator last, unsigned int& out_num);
	static const bool extract_PtgRefErr(std::wstring::const_iterator& first, std::wstring::const_iterator last);
	static const bool extract_PtgErr(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str);
	static const bool extract_UndefinedName(std::wstring::const_iterator& first, std::wstring::const_iterator last);

	static const bool extract_PtgArea(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str);
	static const bool extract_PtgRef(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str);
	static const bool extract_3D_part(std::wstring::const_iterator& first, std::wstring::const_iterator last, unsigned short& ixti);

	static const bool extract_PtgArray(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str);

	static const bool extract_LeftParenthesis(std::wstring::const_iterator& first, std::wstring::const_iterator last);
	static const bool extract_RightParenthesis(std::wstring::const_iterator& first, std::wstring::const_iterator last);

	static const bool extract_PtgFunc(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str);
};

} // namespace XLS

