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

#include "PIVOTCACHE.h"

#include "SXFORMULA_bu.h"
#include "FDB.h"
#include "DBB.h"
#include "SXOPER.h"
#include "PRFILTER.h"
#include "PIVOTRULE.h"

#include "../Biff_records/SXDB.h"
#include "../Biff_records/SXDBEx.h"
#include "../Biff_records/EOF.h"
#include "../Biff_records/SxItm.h"
#include "../Biff_records/SxFilt.h"

namespace XLS
{

PIVOTCACHE::PIVOTCACHE()
{
}

PIVOTCACHE::~PIVOTCACHE()
{
}


BaseObjectPtr PIVOTCACHE::clone()
{
	return BaseObjectPtr(new PIVOTCACHE(*this));
}

// PIVOTCACHE = SXDB SXDBEx *SXFORMULA *FDB *DBB EOF
const bool PIVOTCACHE::loadContent(BinProcessor& proc)
{
	GlobalWorkbookInfoPtr global_info = proc.getGlobalWorkbookInfo();
	
	global_info->arPivotCacheFieldShortSize.clear();
	global_info->arPivotCacheFields.clear();

	if(!proc.mandatory<SXDB>())
	{
		return false;
	}
	m_SXDB = elements_.back();
	elements_.pop_back();

	if(proc.mandatory<SXDBEx>())
	{
		m_SXDBEx = elements_.back();
		elements_.pop_back();
	}
	int count = 0;
	
	count = proc.repeated<SXFORMULA>(0, 0);
	while(count--)
	{
		m_arSXFORMULA.push_back(elements_.front());	elements_.pop_front();
	}
	
	count = proc.repeated<FDB>(0, 0);
	int i = 0;
	while(count--)
	{
		m_arFDB.push_back(elements_.front());	elements_.pop_front();

		FDB* fdb = dynamic_cast<FDB*>(m_arFDB.back().get());
		fdb->index = i++;
	}

	count = proc.repeated<DBB>(0, 0);
	while(count--)
	{
		m_arDBB.push_back(elements_.front());	elements_.pop_front();
		
		DBB* dbb = dynamic_cast<DBB*>(m_arDBB.back().get());
		
		dbb->arPivotCacheFieldShortSize	= global_info->arPivotCacheFieldShortSize;
		dbb->arPivotCacheFields			= global_info->arPivotCacheFields;
	}
	if (proc.optional<EOF_T>())
	{
		elements_.pop_back();
	}
//--------------------------------------------------------------------------------------------------------------------
	for (size_t i = 0; i < m_arSXFORMULA.size(); i++)
	{
		SXFORMULA* formula = dynamic_cast<SXFORMULA*>(m_arSXFORMULA[i].get());
		if (!formula)continue;
	
		PIVOTRULE* pivot_rule = dynamic_cast<PIVOTRULE*>(formula->m_PIVOTRULE.get());
		if (!pivot_rule) continue;

		for (size_t j = 0; j < pivot_rule->m_arPRFILTER.size(); j++)
		{
			PRFILTER*	filter	= dynamic_cast<PRFILTER*>(pivot_rule->m_arPRFILTER[j].get());
			if (!filter) continue;
			
			SxItm*		item	= dynamic_cast<SxItm*>(filter->m_SxItm.get());
			SxFilt*		filt	= dynamic_cast<SxFilt*>(filter->m_SxFilt.get());
					
			for (size_t i = 0; (item && filt) && i < item->rgisxvi.size(); i++)
			{
				short index_cache = filt->isxvd;
				short index_item = item->rgisxvi[i];
				if (index_cache >= 0 && index_cache < m_arFDB.size())
				{
					FDB* field = dynamic_cast<FDB*>(m_arFDB[index_cache].get());
					if (field)
					{
						if (index_item >= 0 && index_item < field->m_arSRCSXOPER.size())
						{
							SXOPER* cache = dynamic_cast<SXOPER*>(field->m_arSRCSXOPER[index_item].get());
							if (cache)
							{
								cache->bFormula = true;
							}
						}
					}
				}
			}
		}
	}

	return true;
}

} // namespace XLS

