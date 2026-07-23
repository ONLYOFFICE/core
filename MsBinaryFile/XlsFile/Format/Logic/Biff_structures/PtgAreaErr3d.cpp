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

#include "PtgAreaErr3d.h"
#include "RevExtern.h"

namespace XLS
{
PtgAreaErr3d::PtgAreaErr3d(const unsigned short full_ptg_id) : OperandPtg(full_ptg_id)
{
}

BiffStructurePtr PtgAreaErr3d::clone()
{
	return BiffStructurePtr(new PtgAreaErr3d(*this));
}


void PtgAreaErr3d::loadFields(CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();

	record >> ixti;

    if (global_info->Version < 0x0800)
        record.skipNunBytes(8); // unused
    else
        record.skipNunBytes(12); // unused

}

void PtgAreaErr3d::writeFields(CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();

	record << ixti;

	if (global_info->Version < 0x0800)
		record.reserveNunBytes(8); // unused
	else
		record.reserveNunBytes(12); // unused

}

void PtgAreaErr3d::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	RevExternPtr tab_ids;
	if(!extra_data.empty() && (tab_ids = boost::dynamic_pointer_cast<RevExtern>(extra_data.front())))
	{
		Log::info("PtgAreaErr3d struct for revisions is not assemble.");
		ptg_stack.push(L"");
		extra_data.pop();
		return;
	}
	std::wstring link = (ixti < global_info->arXti_External.size()) ? global_info->arXti_External[ixti].link : L"";
	if (!link.empty()) 
		link += L"!";
	
	ptg_stack.push(link + L"#REF!"); // full_ref ???
}


} // namespace XLS

