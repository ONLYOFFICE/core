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

#include "PtgFuncVar.h"

namespace XLS
{


PtgFuncVar::PtgFuncVar(const unsigned short full_ptg_id) : OperandPtg(full_ptg_id)
{
}


PtgFuncVar::PtgFuncVar(const unsigned short func_index, const unsigned char data_type)
: OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5)),
	tab(func_index)
{
}


BiffStructurePtr PtgFuncVar::clone()
{
	return BiffStructurePtr(new PtgFuncVar(*this));
}


void PtgFuncVar::loadFields(CFRecord& record)
{
	global_info_ = record.getGlobalWorkbookInfo();
	
	global_info_->bMacrosExist = true;

	record >> cparams;
	record.loadAnyData(tab);
	fCeFunc = tab.isMacro();
}


void PtgFuncVar::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	std::wstring arguments;
	std::wstring func_name = tab.getFuncName();

	unsigned char nparams = cparams; // To let us change the value temporarily
	if(nparams && 0xFF == tab.getIndex()) // user-defined function
	{
		--nparams;
	}
	
	int sz = ptg_stack.size();
	
	if(nparams && !ptg_stack.empty())
	{
		arguments += ptg_stack.top();
		ptg_stack.pop();
		for(unsigned char i = 0; i < nparams - 1 && !ptg_stack.empty(); ++i)
		{
			arguments = ptg_stack.top() + L',' + arguments;
			ptg_stack.pop();
		}
	}
	if(0xFF == tab.getIndex()&& !ptg_stack.empty()) // user-defined function
	{
		func_name = ptg_stack.top();

		if (	func_name == L"WEEKS"		|| 
				func_name == L"DAYSINYEAR"	|| 
				func_name == L"DAYSINMONTH"	||
				func_name == L"ISLEAPYEAR"	||
				func_name == L"MONTHS"		||
				func_name == L"WEEKSINYEAR"	||
				func_name == L"YEARS")
		{
			func_name = L"_xll." + func_name;
		}
		ptg_stack.pop();
	}
	ptg_stack.push(func_name + L'(' + arguments + L')');
}


// static
PtgPtr PtgFuncVar::create(const std::wstring& word, const unsigned char data_type)
{
	unsigned short func_index = Ftab_Cetab::recognizeFunction(word);
	if(0xFFFF != func_index)
	{
		Ftab_Cetab tab(func_index);
		if(tab.isMacro() || -1 == tab.getParamsNum())
		{
			return PtgPtr(new PtgFuncVar(func_index, data_type));
		}
	}
	return PtgPtr();
}


void PtgFuncVar::setParamsNum(const unsigned char num)
{
	cparams = num;
	if(0xFF == tab.getIndex())
	{
		++cparams;
	}
}


} // namespace XLS

