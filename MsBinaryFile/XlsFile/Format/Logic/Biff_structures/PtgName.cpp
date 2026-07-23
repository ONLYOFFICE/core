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

#include "PtgName.h"
#include "RevNameTabid.h"

namespace XLS
{


PtgName::PtgName(const unsigned short full_ptg_id) : OperandPtg(full_ptg_id)
{
}


PtgName::PtgName(const unsigned int index, const PtgDataType data_type)
:	nameindex(index),
	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5))
{
}


BiffStructurePtr PtgName::clone()
{
	return BiffStructurePtr(new PtgName(*this));
}

void PtgName::loadFields(CFRecord& record)
{
	
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		_UINT16 val;
		record >> val;
		nameindex = val;
		record.skipNunBytes(12);
	}
	else
	{
		record >> nameindex;		
	}
	
	global_info = record.getGlobalWorkbookInfo();
}

void PtgName::writeFields(CFRecord& record)
{
	record << nameindex;

	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		record.reserveNunBytes(12);
	}
    else if(record.getGlobalWorkbookInfo()->Version < 0x0800 )
	{
		//record.reserveNunBytes(2);
	}

	global_info = record.getGlobalWorkbookInfo();
}


void PtgName::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	RevNameTabidPtr tab_id;
	if(!extra_data.empty() && (tab_id = boost::dynamic_pointer_cast<RevNameTabid>(extra_data.front())))
	{
		Log::info("PtgName struct for revisions is not assemble.");
		ptg_stack.push(tab_id->toString());
		extra_data.pop();
		return;
	}

	std::wstring ptg;
	
    if ((global_info) && (nameindex <= global_info->arDefineNames.size()) && nameindex)
	{
		ptg = global_info->arDefineNames[nameindex-1];
	}
	
	if(!ptg.empty())
	{
		ptg_stack.push(ptg);
	}
	else
	{
		ptg_stack.push(L"#UNDEFINED_NAME(" + STR::int2wstr(nameindex) + L")!");
	}
}


} // namespace XLS

