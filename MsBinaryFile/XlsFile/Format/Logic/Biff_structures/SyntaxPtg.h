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
#pragma once

#include "Ptg.h"

namespace XLS
{
class PtgList;
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
	static const bool extract_PtgList(std::wstring::const_iterator& first, std::wstring::const_iterator last, PtgList& ptgList, unsigned short ixti = 0);
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
	static const bool extract_FutureFunction(const std::wstring &funcName, unsigned int& out_num);
	static const void extract_CustomFunction(const std::wstring &funcName, unsigned int& out_num);
	static const void remove_extraSymbols(std::wstring::const_iterator& first, std::wstring::const_iterator& last);
};

} // namespace XLS

