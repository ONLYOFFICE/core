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

#include "PtgSxName.h"

#include "../Biff_unions/PIVOTCACHE.h"
#include "../Biff_unions/FDB.h"
#include "../Biff_unions/SXOPER.h"

#include "../Biff_records/SXFDB.h"
#include "../Biff_records/SxName.h"
#include "../Biff_records/SXPair.h"
#include "../../../../../OOXML/XlsbFormat/Biff12_records/BeginPName.h"


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

void PtgSxName::writeFields(CFRecord& record)
{
	record << sxIndex;

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
		else if(static_cast<XLSB::BeginPName*>(global_info->arPivotSxNames[sxIndex].name.get()))
		{
			auto pname = static_cast<XLSB::BeginPName*>(global_info->arPivotSxNames[sxIndex].name.get());
			if(pname->ifdb >=0 && pname->ifdb < global_info->arPivotCacheSxNames.size())
			{
				_Name = global_info->arPivotCacheSxNames[pname->ifdb];
                if (std::wstring::npos != _Name.find(L" "))
                {
                    _Name = L"'" + _Name + L"'";
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

