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

#include "PtgParam.h"

#include "../../Auxiliary/HelpFunc.h"

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


const int PtgParam::getFirstParam() const
{
	return params[0];
}


const int PtgParam::getSecondParam() const
{
	return params[1];
}


const int PtgParam::getParam(const size_t pos) const
{
	return params[pos];
}


const int PtgParam::getParamsNum() const
{
	return params.size();
}


void PtgParam::addParam(const int param)
{
	params.push_back(param);
}


const std::wstring PtgParam::toString() const
{
	std::wstring result = L'#' + type2str(type_);
	if(!params.empty())
	{
		result += L'(';
        for(std::vector<int>::const_iterator it = params.begin(), itEnd = --params.end(); it != itEnd; ++it)
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

