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

#include "PIVOTVIEWEX.h"
#include "PIVOTTH.h"
#include "PIVOTVDTEX.h"

#include "../Biff_records/SXTH.h"
#include "../Biff_records/SXViewEx.h"
#include "../Biff_records/SXPIEx.h"
#include "../Biff_records/SXVDTEx.h"

namespace XLS
{

PIVOTVIEWEX::PIVOTVIEWEX()
{
}

PIVOTVIEWEX::~PIVOTVIEWEX()
{
}

BaseObjectPtr PIVOTVIEWEX::clone()
{
	return BaseObjectPtr(new PIVOTVIEWEX(*this));
}

// PIVOTVIEWEX = SXViewEx *PIVOTTH *SXPIEx *PIVOTVDTEX
const bool PIVOTVIEWEX::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXViewEx>())
	{
		return false;
	}
	m_SXViewEx = elements_.back();	elements_.pop_back();
	
	int count = 0;
	count = proc.repeated<PIVOTTH>(0, 0);
	while(count--)
	{
		m_arPIVOTTH.push_back(elements_.front());	elements_.pop_front();
	}

	count = proc.repeated<SXPIEx>(0, 0);
	while(count--)
	{
		m_arSXPIEx.push_back(elements_.front());	elements_.pop_front();
	}

	count = proc.repeated<PIVOTVDTEX>(0, 0);
	while(count--)
	{
		m_arPIVOTVDTEX.push_back(elements_.front());	elements_.pop_front();
	}

	return true;
}
int PIVOTVIEWEX::serialize(std::wostream & strm)
{				
	if (m_arPIVOTTH.empty()) return 0;

	std::unordered_map<std::wstring, std::vector<int>> mapDimensions;
	std::unordered_map<std::wstring, std::vector<int>> mapMeasures;
	std::unordered_map<std::wstring, std::vector<int>> mapKpis;
	std::unordered_map<std::wstring, std::vector<int>> mapNamed;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"cacheHierarchies")
		{
			CP_XML_ATTR(L"count", m_arPIVOTTH.size());
			
			for (size_t i = 0; i < m_arPIVOTTH.size(); i++)
			{
				PIVOTTH* ht = dynamic_cast<PIVOTTH*>(m_arPIVOTTH[i].get());
				SXTH* szTH = dynamic_cast<SXTH*>(ht->m_SXTH.get());
				
				if (szTH->fKPI)	
				{
					std::unordered_map<std::wstring, std::vector<int>>::iterator pFind = mapKpis.find(szTH->stUnique.value());
					if (pFind == mapKpis.end())
					{
						std::vector<int> v; v.push_back(i);
						mapKpis.insert(std::make_pair(szTH->stUnique.value(), v));
					}
					else
					{
						pFind->second.push_back(i);
					}
				}
				else if (szTH->fMeasure && !szTH->fSet && !szTH->fKPI)
				{
					std::unordered_map<std::wstring, std::vector<int>>::iterator pFind = mapMeasures.find(szTH->stUnique.value());
					if (pFind == mapMeasures.end())
					{
						std::vector<int> v; v.push_back(i);
						mapMeasures.insert(std::make_pair(szTH->stUnique.value(), v));
					}
					else
					{
						pFind->second.push_back(i);
					}
				}
				else if (szTH->fSet && !szTH->fMeasure && !szTH->fKPI)
				{
					std::unordered_map<std::wstring, std::vector<int>>::iterator pFind = mapNamed.find(szTH->stUnique.value());
					if (pFind == mapNamed.end())
					{
						std::vector<int> v; v.push_back(i);
						mapNamed.insert(std::make_pair(szTH->stUnique.value(), v));
					}
					else
					{
						pFind->second.push_back(i);
					}
				}
				if (!szTH->stDimension.value().empty())
				{
					std::unordered_map<std::wstring, std::vector<int>>::iterator pFind = mapDimensions.find(szTH->stDimension.value());
					if (pFind == mapDimensions.end())
					{
						std::vector<int> v; v.push_back(i);
						mapDimensions.insert(std::make_pair(szTH->stDimension.value(), v));
					}
					else
					{
						pFind->second.push_back(i);
					}
				}
				
				szTH->serialize(CP_XML_STREAM());
			}
		}
		CP_XML_NODE(L"kpis")
		{
			CP_XML_ATTR(L"count", mapKpis.size());
			
			for (std::unordered_map<std::wstring, std::vector<int>>::iterator it = mapKpis.begin(); it != mapKpis.end(); it++)
			{
				CP_XML_NODE(L"kpi")
				{
					CP_XML_ATTR(L"uniqueName",	it->first);
					//measureGroup
					//value
					//goal
					//status
					//trend
					//weight
					//time
					//parent
				}
			}		
		}
		CP_XML_NODE(L"dimensions")
		{
			CP_XML_ATTR(L"count", mapDimensions.size());
			for (std::unordered_map<std::wstring, std::vector<int>>::iterator it = mapDimensions.begin(); it != mapDimensions.end(); it++)
			{
				CP_XML_NODE(L"dimension")
				{
					CP_XML_ATTR(L"name",		it->first.substr(1, it->first.length() - 2));
					CP_XML_ATTR(L"uniqueName",	it->first);
					
					//if (szTH->fMeasure)
					//	CP_XML_ATTR(L"measure",	true);
				}
			}
		}
		CP_XML_NODE(L"measureGroups")
		{
			CP_XML_ATTR(L"count", mapMeasures.size());
			for (std::unordered_map<std::wstring, std::vector<int>>::iterator it = mapMeasures.begin(); it != mapMeasures.end(); it++)
			{
				CP_XML_ATTR(L"name", it->first.substr(1, it->first.length() - 2));

			}
			//CP_XML_ATTR(L"count", m_arPIVOTVDTEX.size());
			//for (size_t i = 0; i < m_arPIVOTVDTEX.size(); i++)
			//{
			//	PIVOTVDTEX *VDTEX = dynamic_cast<PIVOTVDTEX*>(m_arPIVOTVDTEX[i].get());
			//	SXVDTEx  *ex = dynamic_cast<SXVDTEx*>(VDTEX->m_SXVDTEx.get());
			//	
			//	PIVOTTH* ht = dynamic_cast<PIVOTTH*>(m_arPIVOTTH[ex->isxth].get());
			//	SXTH* szTH = dynamic_cast<SXTH*>(ht->m_SXTH.get());
			//	
			//	CP_XML_NODE(L"measureGroup")
			//	{
			//		CP_XML_ATTR(L"name", szTH->stDimension.value());
			//	}
		
			//}
		}
	}
	return 0;
}
} // namespace XLS

