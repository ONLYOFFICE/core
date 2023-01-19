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

#include "PtgSxName.h"

#include "../Biff_unions/PIVOTCACHE.h"
#include "../Biff_unions/FDB.h"
#include "../Biff_unions/SXOPER.h"

#include "../Biff_records/SXFDB.h"
#include "../Biff_records/SxName.h"
#include "../Biff_records/SXPair.h"

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
	std::wstring _Name;

	if (sxIndex < global_info->arPivotSxNames.size())
	{
		SxName *name = dynamic_cast<SxName*>(global_info->arPivotSxNames[sxIndex].name.get());
			
		if ((name) && (name->ifdb >= 0  && name->ifdb < global_info->arPivotCacheSxNames.size()))
		{
			_Name = global_info->arPivotCacheSxNames[name->ifdb];
			if (std::wstring::npos != _Name.find(L" "))
			{
				_Name = L"'" + _Name + L"'";
			}
		}
		else if (!global_info->arPivotSxNames[sxIndex].pair.empty())
		{
			SXPair *pair = dynamic_cast<SXPair*>(global_info->arPivotSxNames[sxIndex].pair[0].get());
			if (pair)
			{
				std::map<int, BaseObjectPtr>::iterator pFind = global_info->mapPivotCacheStream.find(global_info->idPivotCache);
				if (pFind != global_info->mapPivotCacheStream.end())
				{
					PIVOTCACHE* pivot_cache = dynamic_cast<PIVOTCACHE*>(pFind->second.get());
					if (pivot_cache)
					{
						if (pair->isxvd >= 0 && pair->isxvd < pivot_cache->m_arFDB.size())
						{
							FDB* field = dynamic_cast<FDB*>(pivot_cache->m_arFDB[pair->isxvd].get());
							if (field)
							{

								SXFDB* field_db= dynamic_cast<SXFDB*>(field->m_SXFDB.get());
								if (field_db)
								{
									_Name = field_db->stFieldName.value();

									if (std::wstring::npos != _Name.find(L" ") || 
										std::wstring::npos != _Name.find(L":"))
									{
										_Name = L"'" + _Name + L"'";
									}
								}
								if (pair->iCache >= 0 && pair->iCache < field->m_arSRCSXOPER.size())
								{
									SXOPER* cache = dynamic_cast<SXOPER*>(field->m_arSRCSXOPER[pair->iCache].get());
									if (cache)
									{
										std::wstring value = cache->value;
										if (std::wstring::npos != value.find(L" ") || 
											std::wstring::npos != value.find(L":"))
										{
											value = L"'" + value + L"'";
										}
										_Name += L"[" + value + L"]";
									}
								}
							}
						}
					}
				}
			}
		}
		ptg_stack.push(_Name);
	}
	else
	{
 		Log::warning("PtgSxName structure is not assemble.");

		ptg_stack.push(L""); // This would let us to continue without an error
	}
	
}


} // namespace XLS

