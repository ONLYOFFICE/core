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

#include "PtgFunc.h"

namespace XLS
{


PtgFunc::PtgFunc(const unsigned short full_ptg_id) : OperandPtg(full_ptg_id)
{
}


PtgFunc::PtgFunc(const unsigned short func_index, const unsigned char data_type)
: OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5)),
	iftab(func_index)
{
}

BiffStructurePtr PtgFunc::clone()
{
	return BiffStructurePtr(new PtgFunc(*this));
}

void PtgFunc::loadFields(CFRecord& record)
{
	record >> iftab;
}

void PtgFunc::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
    int num_params = iftab.getParamsNum();
	std::wstring arguments;
	
	if(num_params && ptg_stack.size() > 0)
	{
		arguments += ptg_stack.top();
		ptg_stack.pop();
        for(int i = 0; i < num_params - 1 && ptg_stack.size() > 0; ++i)
		{
			arguments = ptg_stack.top() + L',' + arguments;
			ptg_stack.pop();
		}
	}
	ptg_stack.push(iftab.getFuncName() + L'(' + arguments + L')');

}


// static
PtgPtr PtgFunc::create(const std::wstring& word, const unsigned char data_type)
{
	unsigned short func_index = Ftab_Cetab::recognizeFunction(word);
	if(0xFFFF != func_index)
	{
		Ftab_Cetab tab(func_index);
		if(!tab.isMacro() && -1 != tab.getParamsNum())
		{
			return PtgPtr(new PtgFunc(func_index, data_type));
		}
	}
	return PtgPtr();
}


const int PtgFunc::getParametersNum() const
{
	return iftab.getParamsNum();
}


} // namespace XLS

