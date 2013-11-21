#include "stdafx.h"
#include "SyntaxPtg.h"
#include "OperatorPtgs.h"
#include "PtgParen.h"
#include <boost/regex.hpp>
//#include <boost/algorithm/string/regex.hpp>
namespace XLS
{;

// static
const bool SyntaxPtg::is_operators(std::wstring::const_iterator first, std::wstring::const_iterator last)
{
	static boost::wregex reg_operators(L"^ *[-+*/^&%<=>:]");
	return boost::regex_search(first, last, reg_operators);
}


// static
const bool SyntaxPtg::extract_PtgAdd(std::wstring::const_iterator& first, std::wstring::const_iterator last, const bool operand_expected)
{
	if(!operand_expected)
	{
		static boost::wregex reg_add(L"^ *\\+ *");
		boost::match_results<std::wstring::const_iterator> results;
		if(boost::regex_search(first, last, results, reg_add))
		{
			first = results[0].second;
			return true;
		}
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgUplus(std::wstring::const_iterator& first, std::wstring::const_iterator last, const bool operand_expected)
{
	if(operand_expected)
	{
		static boost::wregex reg_uplus(L"^ *\\+ *");
		boost::match_results<std::wstring::const_iterator> results;
		if(boost::regex_search(first, last, results, reg_uplus))
		{
			first = results[0].second;
			return true;
		}
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgSub(std::wstring::const_iterator& first, std::wstring::const_iterator last, const bool operand_expected)
{
	if(!operand_expected)
	{
		static boost::wregex reg_sub(L"^ *- *");
		boost::match_results<std::wstring::const_iterator> results;
		if(boost::regex_search(first, last, results, reg_sub))
		{
			first = results[0].second;
			return true;
		}
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgUminus(std::wstring::const_iterator& first, std::wstring::const_iterator last, const bool operand_expected)
{
	if(operand_expected)
	{
		static boost::wregex reg_uminus(L"^ *- *");
		boost::match_results<std::wstring::const_iterator> results;
		if(boost::regex_search(first, last, results, reg_uminus))
		{
			first = results[0].second;
			return true;
		}
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgPercent(std::wstring::const_iterator& first, std::wstring::const_iterator last, const bool operand_expected)
{
	static boost::wregex reg_perc(L"^ *% *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_perc))
	{
		OperatorPtgPtr last_operator;
		if(!operand_expected)
		{
			first = results[0].second;
			return true;
		}
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgMul(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_mul(L"^ *\\* *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_mul))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgDiv(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_div(L"^ */ *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_div))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgPower(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_pow(L"^ *\\^ *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_pow))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgEq(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_eq(L"^ *= *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_eq))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgNe(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_ne(L"^ *<> *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_ne))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgLe(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_le(L"^ *<= *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_le))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgLt(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_lt(L"^ *< *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_lt))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgGe(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_ge(L"^ *>= *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_ge))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgGt(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_gt(L"^ *> *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_gt))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgConcat(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_concat(L"^ *& *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_concat))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgUnion(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_union(L"^ *, *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_union))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::is_PtgIsect(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_before_comma(L"^ *[,()]");
	static boost::wregex reg_isect(L"^ ");
	return !boost::regex_search(first, last, reg_before_comma) &&
			boost::regex_search(first, last, reg_isect);
}


// static
const bool SyntaxPtg::extract_PtgIsect(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	if(is_PtgIsect(first, last))
	{
		++first;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgRange(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_range(L"^ *: *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_range))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_comma(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_comma(L"^ *, *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_comma))
	{
		first = results[0].second;
		return true;
	}
	return false;
}

// static
const bool SyntaxPtg::extract_semicolon(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_semi(L"^ *; *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_semi))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgInt(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str)
{
	static boost::wregex reg_differ(L"^\\d+[.:]"); // differ from PtgNum(5.55) and PtgRef(7:7)
	boost::match_results<std::wstring::const_iterator> results;
	if(!boost::regex_search(first, last, reg_differ))
	{
		static boost::wregex reg_int(L"^\\d+");
		if(boost::regex_search(first, last, results, reg_int))
		{
			std::wstring extracted(first, results[0].second);
			if(5 > extracted.length() || 5 == extracted.length() && L"65535" >= extracted)
			{
				out_str = extracted;
				first = results[0].second;
				return true;
			}
		}
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgNum(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str)
{
	static boost::wregex reg_num(L"^[+-]?\\d*(\\d|\\.)\\d*([eE][+-]?\\d+)?");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_num))
	{
		out_str = std::wstring(first, results[0].second);
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgBool(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str)
{
	static boost::wregex reg_bool(L"^(?i)(TRUE|FALSE)([-+*/^&%<=>: ;),]|$)"); // searched case insensitive
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_bool))
	{
		out_str = std::wstring(first, results[1].second);
		first = results[1].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgStr(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str)
{
	static boost::wregex reg_str(L"^\"((\"\"|[^\"])*)\"");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_str))
	{
		out_str = boost::algorithm::replace_all_copy(results.str(1), L"\"\"", L"\"");
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgName(std::wstring::const_iterator& first, std::wstring::const_iterator last, DWORD& out_num, MSXML2::IXMLDOMElementPtr parent)
{
	static boost::wregex reg_name(L"^(\\w[\\w\\d.]*)([-+*/^&%<=>: ;),]|$)");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_name))
	{
		std::wstring name(results.str(1));
		_bstr_t query(L"root/WorkbookStreamObject/GlobalsSubstream/LBL/Lbl");
		MSXML2::IXMLDOMNodeListPtr nodes = parent->GetownerDocument()->selectNodes(query);
		MSXML2::IXMLDOMElementPtr lbl;
		
		for(size_t counter = 1; (lbl = nodes->nextNode()); ++counter)
		{
			MSXML2::IXMLDOMAttributePtr lbl_name(lbl->getAttributeNode(L"Name"));
			if(lbl_name && boost::algorithm::to_upper_copy(name) == 
				boost::algorithm::to_upper_copy(std::wstring(static_cast<wchar_t*>(static_cast<_bstr_t>(lbl_name->Getvalue())))))
			{
				out_num = counter;
				first = results[1].second;
				return true;
			}
		}
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgRefErr(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_err(L"^(?i)#REF!");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_err))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgErr(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str)
{
	static boost::wregex reg_err(L"^(?i)(#NULL!|#DIV/0!|#VALUE!|#REF!|#NAME\\?|#NUM!|#N/A)");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_err))
	{
		out_str = std::wstring(first, results[0].second);
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgArea(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str)
{
	static boost::wregex reg_full(L"^\\$?[A-Za-z]+\\$?\\d+:\\$?[A-Za-z]+\\$?\\d+");
	static boost::wregex reg_columns(L"^\\$?[A-Za-z]+:\\$?[A-Za-z]+");
	static boost::wregex reg_rows(L"^\\$?\\d+:\\$?\\d+");
	boost::match_results<std::wstring::const_iterator> results;
	if (boost::regex_search(first, last, results, reg_full) ||
		boost::regex_search(first, last, results, reg_columns) ||
		boost::regex_search(first, last, results, reg_rows))
	{
		out_str = std::wstring(first, results[0].second);
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgRef(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str)
{
	static boost::wregex reg_ref(L"^(\\$?[A-Za-z]+\\$?\\d+)([-+*/^&%<=>: ;),]|$)");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_ref))
	{
		out_str = std::wstring(first, results[1].second);
		first = results[1].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_3D_part(std::wstring::const_iterator& first, std::wstring::const_iterator last, WORD& ixti, MSXML2::IXMLDOMElementPtr parent)
{
	static boost::wregex reg_sheets(L"^(\\w[\\w\\d.]*(:\\w[\\w\\d.]*)?)!");
	static boost::wregex reg_quoted(L"^'((''|[^]['\\/*?])*)'!");
	boost::match_results<std::wstring::const_iterator> results;
	if (boost::regex_search(first, last, results, reg_sheets) ||
		boost::regex_search(first, last, results, reg_quoted))
	{

		std::wstring sheets_names = results.str(1);

		ixti = XMLSTUFF::sheetsnames2ixti(boost::algorithm::replace_all_copy(sheets_names, L"''", L"'"), parent->GetownerDocument());
		if(0xFFFF != ixti)
		{
			first = results[0].second;
			return true;
		}
	}
	return false;
}


// static
const bool SyntaxPtg::extract_UndefinedName(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_undef(L"^([\\w\\d.]+)([-+*/^&%<=>: ;),]|$)");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_undef))
	{
		// Not compares to the defined names since it has already been done in extract_PtgName
		first = results[1].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgArray(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str)
{
	static boost::wregex reg_array(L"^\\{([^{}\"]+|\"((\"\"|[^\"])*)\")([,;]([^{}\"]+|\"((\"\"|[^\"])*)\"))*\\}");
	boost::match_results<std::wstring::const_iterator> results;
	if (boost::regex_search(first, last, results, reg_array))
	{
		out_str = std::wstring(first, results[0].second);
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_LeftParenthesis(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_left_par(L"^ *\\( *");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_left_par))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_RightParenthesis(std::wstring::const_iterator& first, std::wstring::const_iterator last)
{
	static boost::wregex reg_right_par(L"^ *\\)");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_right_par))
	{
		first = results[0].second;
		return true;
	}
	return false;
}


// static
const bool SyntaxPtg::extract_PtgFunc(std::wstring::const_iterator& first, std::wstring::const_iterator last, std::wstring& out_str)
{
	static boost::wregex reg_func(L"^([\\w\\d.]+)\\(");
	boost::match_results<std::wstring::const_iterator> results;
	if(boost::regex_search(first, last, results, reg_func))
	{
		out_str = std::wstring(first, results[1].second);
		first = results[1].second;
		return true;
	}
	return false;
}


} // namespace XLS

