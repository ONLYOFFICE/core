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

void PtgFunc::writeFields(CFRecord& record)
{
	record << iftab;
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

const unsigned short PtgFunc::getFuncIndex() const
{
	return iftab.getIndex();
}

} // namespace XLS

