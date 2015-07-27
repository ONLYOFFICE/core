#pragma once

#include "Ptg.h"

namespace XLS
{;

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

