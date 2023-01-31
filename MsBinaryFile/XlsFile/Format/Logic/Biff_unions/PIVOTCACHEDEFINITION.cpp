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

#include "PIVOTCACHEDEFINITION.h"
#include "PIVOTCACHE.h"
#include "SXSRC.h"
#include "SXADDLCACHE.h"
#include "FDB.h"
#include "DBQUERY.h"
#include "PIVOTVIEWEX.h"
#include "PIVOTVDTEX.h"

#include "../Biff_records/SXStreamID.h"
#include "../Biff_records/SXVS.h"
#include "../Biff_records/SXDB.h"
#include "../Biff_records/SXDBEx.h"

namespace XLS
{


PIVOTCACHEDEFINITION::PIVOTCACHEDEFINITION()
{
}


PIVOTCACHEDEFINITION::~PIVOTCACHEDEFINITION()
{
}


BaseObjectPtr PIVOTCACHEDEFINITION::clone()
{
	return BaseObjectPtr(new PIVOTCACHEDEFINITION(*this));
}


// PIVOTCACHEDEFINITION = SXStreamID SXVS [SXSRC] [SXADDLCACHE]
const bool PIVOTCACHEDEFINITION::loadContent(BinProcessor& proc)
{
	global_info_ = proc.getGlobalWorkbookInfo();
	
	if(!proc.mandatory<SXStreamID>())
	{
		return false;
	}
	m_SXStreamID = elements_.back();
	elements_.pop_back();

	SXStreamID* streamId = dynamic_cast<SXStreamID*>(m_SXStreamID.get());
	if (!streamId) return 0;

	global_info_->mapPivotCacheIndex.insert(std::make_pair(streamId->idStm, global_info_->mapPivotCacheIndex.size()));

	if (proc.mandatory<SXVS>())
	{
		m_SXVS = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<SXSRC>())
	{
		m_SXSRC = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<SXADDLCACHE>())
	{
		m_SXADDLCACHE = elements_.back();
		elements_.pop_back();
	}

	return true;
}
int PIVOTCACHEDEFINITION::serialize_definitions(std::wostream & strm)
{
	SXStreamID* streamId = dynamic_cast<SXStreamID*>(m_SXStreamID.get());
	if (!streamId) return 0;

	global_info_->arPivotCacheSxNames.clear();
	global_info_->arPivotSxNames.clear();
	global_info_->idPivotCache = streamId->idStm;
	
	std::map<int, BaseObjectPtr>::iterator pFind = global_info_->mapPivotCacheStream.find(streamId->idStm);
	if (pFind == global_info_->mapPivotCacheStream.end()) return 0;

	PIVOTCACHE* pivot_cache = dynamic_cast<PIVOTCACHE*>(pFind->second.get());
	if (!pivot_cache) return 0;

	SXDB*	db		= dynamic_cast<SXDB*>(pivot_cache->m_SXDB.get());
	SXDBEx*	db_ex	= dynamic_cast<SXDBEx*>(pivot_cache->m_SXDBEx.get());

	if (!db || !db_ex)return 0;

	SXSRC* src = dynamic_cast<SXSRC*>(m_SXSRC.get());
	bool bSql = src ? src->bSql : false;
	bool bOLAP = src ? src->bOLAP : false;

	DBQUERY * db_query = src ? dynamic_cast<DBQUERY *>(src->m_source.get()) : NULL;
	if (db_query)
	{
		std::map<int, BaseObjectPtr>::iterator pFind = global_info_->mapIdConnection.find(streamId->idStm);
		if (pFind != global_info_->mapIdConnection.end())
		{
			db_query->m_DConn = pFind->second;
		}
	}
	PIVOTVIEWEX *olap_view = bOLAP ? dynamic_cast<PIVOTVIEWEX *>(m_PIVOTVIEWEX.get()) : NULL;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"pivotCacheDefinition")
		{          
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
            CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		
			if (pivot_cache->m_arDBB.empty() && bSql)
			{
				CP_XML_ATTR(L"saveData", 0);
			}
			else 
			{
				CP_XML_ATTR(L"r:id", L"rId1" );
			}
			CP_XML_ATTR(L"enableRefresh",	1);
			CP_XML_ATTR(L"refreshedBy",		db->rgb.value());
			CP_XML_ATTR(L"refreshedDate",	db_ex->numDate.data.value);
			CP_XML_ATTR(L"recordCount",		db->crdbdb);

			if (src)
			{
				src->serialize(CP_XML_STREAM());
			}
			
			CP_XML_NODE(L"cacheFields")
			{
				CP_XML_ATTR(L"count", pivot_cache->m_arFDB.size());

				for (size_t i = 0; i < pivot_cache->m_arFDB.size(); i++)
				{
					FDB *field = dynamic_cast<FDB *>(pivot_cache->m_arFDB[i].get());
					if (!field) continue;
					
					if (olap_view)
					{
						PIVOTVDTEX *ex = dynamic_cast<PIVOTVDTEX*>(olap_view->m_arPIVOTVDTEX[i].get());
						
						field->m_SXVDTEx	= ex->m_SXVDTEx;
						field->m_arPIVOTTH	= olap_view->m_arPIVOTTH;
					}

					field->serialize(CP_XML_STREAM(), bSql, !pivot_cache->m_arDBB.empty());
				}
			}
			if (!pivot_cache->m_arSXFORMULA.empty())
			{
				CP_XML_NODE(L"calculatedItems")
				{
					CP_XML_ATTR(L"count", pivot_cache->m_arSXFORMULA.size());

					for (size_t i = 0; i < pivot_cache->m_arSXFORMULA.size(); i++)
					{
						pivot_cache->m_arSXFORMULA[i]->serialize(CP_XML_STREAM());
					}
				}
			}
			if (olap_view)
			{
				olap_view->m_PIVOTADDL = m_PIVOTADDL;

				olap_view->serialize(CP_XML_STREAM());
			}
		}
	}
	return 0;
}


int PIVOTCACHEDEFINITION::serialize_records(std::wostream & strm)
{
	SXStreamID* streamId = dynamic_cast<SXStreamID*>(m_SXStreamID.get());
	if (!streamId) return 0;

	std::map<int, BaseObjectPtr>::iterator pFind = global_info_->mapPivotCacheStream.find(streamId->idStm);
	if (pFind == global_info_->mapPivotCacheStream.end()) return 0;

	PIVOTCACHE* pivot_cache = dynamic_cast<PIVOTCACHE*>(pFind->second.get());
	if (!pivot_cache) return 0;

	SXSRC* src = dynamic_cast<SXSRC*>(m_SXSRC.get());
	bool bSql = src ? src->bSql : false;

	if (pivot_cache->m_arDBB.empty() && bSql) return 0;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"pivotCacheRecords")
		{          
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
            CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		
			if (!pivot_cache->m_arDBB.empty())
			{
				CP_XML_ATTR(L"count", pivot_cache->m_arDBB.size());
				for (size_t i = 0; i < pivot_cache->m_arDBB.size(); i++)
				{
					pivot_cache->m_arDBB[i]->serialize(CP_XML_STREAM());
				}
			}
			else
			{
				CP_XML_ATTR(L"count", pivot_cache->m_arFDB.size());
				for (size_t i = 0; i < pivot_cache->m_arFDB.size(); i++)
				{
					FDB* fdb = dynamic_cast<FDB*>(pivot_cache->m_arFDB[i].get());
					fdb->serialize_record(CP_XML_STREAM());
				}
			}
		}
	}
	return 0;
}


} // namespace XLS

