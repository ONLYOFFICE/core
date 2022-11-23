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
	record >> nameindex;
	
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		record.skipNunBytes(12);
	}
	else
	{
		record.skipNunBytes(2);
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
	
	if ((global_info) && (nameindex <= global_info->arDefineNames.size()))
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

