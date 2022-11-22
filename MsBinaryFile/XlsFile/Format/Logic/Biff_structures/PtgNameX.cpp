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

#include "PtgNameX.h"
#include "RevName.h"

namespace XLS
{


PtgNameX::PtgNameX(const unsigned short full_ptg_id) : OperandPtg(full_ptg_id)
{
}


PtgNameX::PtgNameX(const std::wstring& word, const PtgDataType data_type)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5))
{
	fromString(word);
}


BiffStructurePtr PtgNameX::clone()
{
	return BiffStructurePtr(new PtgNameX(*this));
}

void PtgNameX::loadFields(CFRecord& record)
{
	record >> ixti;
	
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		record.skipNunBytes(8);
		
		_UINT16 val;
		record >> val;

		nameindex = val;
		record.skipNunBytes(12);
	}
	else
		record >> nameindex;

	global_info = record.getGlobalWorkbookInfo();
}


void PtgNameX::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	RevNamePtr tab_id;
	
	if(!extra_data.empty() && (tab_id = boost::dynamic_pointer_cast<RevName>(extra_data.front())))
	{
		Log::error("PtgNameX struct for revisions is not assemble.");
		ptg_stack.push(L"#REF!");
		extra_data.pop();
		return;
	}
	std::wstring link;
	std::wstring name;
	if(ixti >= 0 && ixti < global_info->arXti_External.size())
	{
		link = global_info->arXti_External[ixti].link;

		if ((global_info->arXti_External[ixti].pNames) && (nameindex > 0 && nameindex <= global_info->arXti_External[ixti].pNames->size()))
		{
			name = global_info->arXti_External[ixti].pNames->at(nameindex - 1);
		}

	}
	if (name.empty())
	{
		if (nameindex <= global_info->arDefineNames.size())
		{
			name = global_info->arDefineNames[nameindex - 1];

			std::map<std::wstring, std::vector<std::wstring>>::iterator pFind = global_info->mapDefineNames.find(name);
			if (link.empty() && full_ref && pFind != global_info->mapDefineNames.end())
			{
				if (ixti < pFind->second.size() && ixti >= 0)
				{
					if (!pFind->second[ixti].empty())
						name = pFind->second[ixti]; //значение 
				}
				else
				{
					for (size_t i = 0; i < pFind->second.size(); i++)
					{
						if (pFind->second[i].empty() == false)
						{
							link = L"[" + std::to_wstring(i) + L"]";
							break;
						}
					}
				}
			}
		}
		else
		{
 			Log::warning("PtgNameX is not yet assemble!! Waiting end parsing defined names");
		}	
	}
	if (!link.empty() && !name.empty())
	{
		ptg_stack.push(link + L"!" + name);
	}
	else if (!name.empty())
	{
		ptg_stack.push(name);
	}
	else
	{
		ptg_stack.push(link);		
	}
}


void PtgNameX::fromString(const std::wstring& word)
{
 	std::wstring  query = L"root/WorkbookStreamObject/GlobalsSubstream/SUPBOOK";
}


} // namespace XLS

