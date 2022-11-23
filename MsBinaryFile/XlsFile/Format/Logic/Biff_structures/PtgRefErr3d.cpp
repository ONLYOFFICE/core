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

#include "PtgRefErr3d.h"
#include "RevExtern.h"

namespace XLS
{


PtgRefErr3d::PtgRefErr3d(const unsigned short full_ptg_id) : OperandPtg(full_ptg_id)
{
}


PtgRefErr3d::PtgRefErr3d(const unsigned short ixti_init, const PtgDataType data_type)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5)),
	ixti(ixti_init)
{
}


BiffStructurePtr PtgRefErr3d::clone()
{
	return BiffStructurePtr(new PtgRefErr3d(*this));
}

void PtgRefErr3d::loadFields(CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();

	record >> ixti;

    if (global_info->Version < 0x0800)
        record.skipNunBytes(4); // unused
    else
        record.skipNunBytes(6); // unused
}


void PtgRefErr3d::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	RevExternPtr tab_ids;
	if(!extra_data.empty() && (tab_ids = boost::dynamic_pointer_cast<RevExtern>(extra_data.front())))
	{
		Log::info("PtgRefErr3d struct for revisions is not assemble.");

		ptg_stack.push(L"");
		extra_data.pop();
		return;
	}

	if (ixti != 0xffff)
	{
		std::wstring link = global_info->arXti_External[ixti].link;
		if (!link.empty()) 
			link += L"!";
		
		ptg_stack.push(link + L"#REF!"); // full_ref ???
	}
}


} // namespace XLS

