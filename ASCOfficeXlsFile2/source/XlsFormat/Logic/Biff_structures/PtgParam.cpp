
#include "PtgParam.h"

#include "Auxiliary/HelpFunc.h"

namespace XLS
{

PtgParam::PtgParam(const std::wstring unparsed_line)
:	type_(ptNONE)
{
	if(L'#' != unparsed_line[0])
	{
		return;
	}

	const size_t end_of_tag = unparsed_line.find_first_of(L"!(");
	if(std::wstring::npos == end_of_tag)
	{
		return;
	}
	const std::wstring type_str = unparsed_line.substr(1, end_of_tag - 1);
	if(ptNONE == (type_ = str2type(type_str)))
	{
		return;
	}
	if(L'(' == unparsed_line[end_of_tag])
	{
		const size_t last_paren = unparsed_line.find_first_of(L")");
		if(std::wstring::npos != last_paren)
		{
			size_t param_pos = end_of_tag + 1;
			while(last_paren > param_pos)
			{
				const size_t end_of_param = unparsed_line.find_first_of(L"),");

                int lVal = 0;

#if defined(_WIN32) || defined (_WIN64)
                 lVal = _wtol(unparsed_line.substr(param_pos, end_of_param - param_pos).c_str());
#else
                wchar_t * pEnd;
                lVal = wcstol(unparsed_line.substr(param_pos, end_of_param - param_pos).c_str(), &pEnd ,10);
#endif

                params.push_back(lVal);
				param_pos = end_of_param + 1;
			}
		}
	}
	const size_t excl_place = unparsed_line.find_first_of(L"!");
	if(std::wstring::npos == excl_place)
	{
		return;
	}
	clear_line_ = unparsed_line.substr(excl_place + 1, unparsed_line.length() - excl_place - 1);
}


PtgParam::PtgParam(const ParamType type, const std::wstring clear_line)
:	type_(type),
	clear_line_(clear_line)
{
}


const PtgParam::ParamType PtgParam::str2type(const std::wstring str) const
{
	if(str == type2str(ptUNDEFINED_NAME))
	{
		return ptUNDEFINED_NAME;
	}
	if(str == type2str(ptCACHE))
	{
		return ptCACHE;
	}
	if(str == type2str(ptELF_RADICAL))
	{
		return ptELF_RADICAL;
	}
	return ptNONE;
}


const std::wstring PtgParam::type2str(const PtgParam::ParamType type) const
{
	switch(type)
	{
		case ptUNDEFINED_NAME:
			return L"UNDEFINED_NAME";

		case ptCACHE:
			return L"CACHE";

		case ptELF_RADICAL:
			return L"ELF_RADICAL";

		default:
			return L"NONE";
	}
}


const PtgParam::ParamType PtgParam::getType() const
{
	return type_;
}


const std::wstring PtgParam::getClearLine() const
{
	return clear_line_;
}


const long PtgParam::getFirstParam() const
{
	return params[0];
}


const long PtgParam::getSecondParam() const
{
	return params[1];
}


const long PtgParam::getParam(const size_t pos) const
{
	return params[pos];
}


const long PtgParam::getParamsNum() const
{
	return params.size();
}


void PtgParam::addParam(const long param)
{
	params.push_back(param);
}


const std::wstring PtgParam::toString() const
{
	std::wstring result = L'#' + type2str(type_);
	if(!params.empty())
	{
		result += L'(';
		for(std::vector<long>::const_iterator it = params.begin(), itEnd = --params.end(); it != itEnd; ++it)
		{
			result += STR::int2wstr(*it) + L',';
		}
		result += STR::int2wstr(params.back());
		result += L')';
	}
	result += L'!';
	return result;
}



} // namespace XLS

