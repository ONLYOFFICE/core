/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "PtgSxName.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgSxName::clone()
{
	return BiffStructurePtr(new PtgSxName(*this));
}

void PtgSxName::loadFields(CFRecord& record)
{
	record >> sxIndex;
	
	global_info = record.getGlobalWorkbookInfo();
}

void PtgSxName::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	//RevNamePtr tab_id;
	//if(!extra_data.empty() && (tab_id = boost::dynamic_pointer_cast<RevName>(extra_data.front())))
	//{
	//	Log::error("PtgNameX struct for revisions is not assemble.");
	//	ptg_stack.push(L"#REF!");
	//	extra_data.pop();
	//	return;
	//}

	std::wstring _Name;
	if(sxIndex > 0 && sxIndex <= global_info->AddinUdfs.size() && !(_Name = global_info->AddinUdfs[sxIndex - 1]).empty())
	{
		ptg_stack.push(_Name);
	}
	else if(sxIndex > 0 && sxIndex <= global_info->xti_parsed.size())
	{
		std::wstring sheet = global_info->xti_parsed[sxIndex-1];

		if (!sheet.empty()) sheet += L"!";
		
		if (sxIndex > 0 && sxIndex <= global_info->arDefineNames.size())
		{
			_Name = global_info->arDefineNames[sxIndex - 1];
		}

		if (sheet.empty() && _Name.empty() && sxIndex <= global_info->arExternalNames.size() && sxIndex > 0)
		{
			_Name = global_info->arExternalNames[sxIndex - 1];
		}

		ptg_stack.push(sheet + _Name);
	}
	else
	{
 		Log::warning("PtgSxName structure is not assemble.");

		ptg_stack.push(L""); // This would let us to continue without an error
	}
	
}


} // namespace XLS

