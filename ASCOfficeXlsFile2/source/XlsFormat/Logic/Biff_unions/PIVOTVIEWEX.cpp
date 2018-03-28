/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
#include "PIVOTADDL.h"

#include "../Biff_records/SXTH.h"
#include "../Biff_records/SXViewEx.h"
#include "../Biff_records/SXPIEx.h"
#include "../Biff_records/SXVDTEx.h"
#include "../Biff_records/SXAddl.h"

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

	std::unordered_map<std::wstring, std::vector<int>>	mapDimensions;
	std::unordered_map<std::wstring, std::vector<int>>	mapMeasures;
	std::unordered_map<std::wstring, int>				mapKpis;
	std::unordered_map<std::wstring, std::vector<int>>	mapNamed;
	
	PIVOTADDL *addls = dynamic_cast<PIVOTADDL*>(m_PIVOTADDL.get());

	bool bAddl = addls ? (addls->m_arSXADDLHIERARCHY.size() == m_arPIVOTTH.size()) : false;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"cacheHierarchies")
		{
			CP_XML_ATTR(L"count", m_arPIVOTTH.size());
			
			for (size_t i = 0; i < m_arPIVOTTH.size(); i++)
			{
				PIVOTTH* th = dynamic_cast<PIVOTTH*>(m_arPIVOTTH[i].get());
				SXTH* sxTH = dynamic_cast<SXTH*>(th->m_SXTH.get());
				
				SXAddl_SXCHierarchy_SXDId			*id				= NULL;
				SXAddl_SXCHierarchy_SXDInfo12		*info12			= NULL;
				SXAddl_SXCHierarchy_SXDUserCaption	*user_caption	= NULL;
				SXAddl_SXCHierarchy_SXDMeasureGrp	*measure_grp	= NULL;
				
				for (size_t j = 0; bAddl && j < addls->m_arSXADDLHIERARCHY[i].elements.size(); j++)
				{
					SXAddl * addl = dynamic_cast<SXAddl*>(addls->m_arSXADDLHIERARCHY[i].elements[j].get());
					if (!addl) continue;

					if (!id)			id				= dynamic_cast<SXAddl_SXCHierarchy_SXDId*>			(addl->content.get());
					if (!info12)		info12			= dynamic_cast<SXAddl_SXCHierarchy_SXDInfo12*>		(addl->content.get());
					if (!user_caption)	user_caption	= dynamic_cast<SXAddl_SXCHierarchy_SXDUserCaption*>	(addl->content.get());
					if (!measure_grp)	measure_grp		= dynamic_cast<SXAddl_SXCHierarchy_SXDMeasureGrp*>	(addl->content.get());
				}
				if (sxTH->fKPI)	
				{
					std::unordered_map<std::wstring, int>::iterator pFind = mapKpis.find(sxTH->stUnique.value());
					if (pFind == mapKpis.end())
					{
						//std::vector<int> v; v.push_back(i);
						mapKpis.insert(std::make_pair(sxTH->stUnique.value(), i/*v*/));
					}
					else
					{
						//pFind->second.push_back(i);
					}
				}
				else if (sxTH->fMeasure && !sxTH->fSet && !sxTH->fKPI && measure_grp)
				{
					std::unordered_map<std::wstring, std::vector<int>>::iterator pFind = mapMeasures.find(measure_grp->stMeasureGroup.string.value());
					if (pFind == mapMeasures.end())
					{
						std::vector<int> v; v.push_back(i);
						mapMeasures.insert(std::make_pair(measure_grp->stMeasureGroup.string.value(), v));
					}
					else
					{
						pFind->second.push_back(i);
					}
				}
				else if (sxTH->fSet && !sxTH->fMeasure && !sxTH->fKPI)
				{
					std::unordered_map<std::wstring, std::vector<int>>::iterator pFind = mapNamed.find(sxTH->stUnique.value());
					if (pFind == mapNamed.end())
					{
						std::vector<int> v; v.push_back(i);
						mapNamed.insert(std::make_pair(sxTH->stUnique.value(), v));
					}
					else
					{
						pFind->second.push_back(i);
					}
				}
				if (!sxTH->stDimension.value().empty())
				{
					std::unordered_map<std::wstring, std::vector<int>>::iterator pFind = mapDimensions.find(sxTH->stDimension.value());
					if (pFind == mapDimensions.end())
					{
						std::vector<int> v; v.push_back(i);
						mapDimensions.insert(std::make_pair(sxTH->stDimension.value(), v));
					}
					else
					{
						pFind->second.push_back(i);
					}
				}
				
				CP_XML_NODE(L"cacheHierarchy")
				{ 
					CP_XML_ATTR(L"uniqueName",				sxTH->stUnique.value());

					if (user_caption)
					{
						CP_XML_ATTR(L"caption",				user_caption->stCaption.string.value());
					}
					else
					{
						CP_XML_ATTR(L"caption",				sxTH->stDisplay.value());
					}
					if (sxTH->fMeasure)
					{
						CP_XML_ATTR(L"measure", true);
						if (measure_grp)
						{
							CP_XML_ATTR(L"measureGroup", measure_grp->stMeasureGroup.string.value());
						}
						if (sxTH->cisxvd > 0)
							CP_XML_ATTR(L"oneField", sxTH->cisxvd);
					}
					else
					{
						//if (!sxTH->stDimension.value().empty())
						//{
						//	CP_XML_ATTR(L"attribute",			sxTH->fKeyAttributeHierarchy); 
						//	CP_XML_ATTR(L"keyAttribute",		sxTH->fKeyAttributeHierarchy); 
						//}
						//else
						{
							CP_XML_ATTR(L"attribute",			!sxTH->fKeyAttributeHierarchy); 
						}
						//keyAttribute
						CP_XML_ATTR(L"defaultMemberUniqueName", sxTH->stDefault.value());
						CP_XML_ATTR(L"allUniqueName",			sxTH->stAll.value());
						CP_XML_ATTR(L"dimensionUniqueName",		sxTH->stDimension.value());

						CP_XML_ATTR(L"count", 0);//levels in this hierarchy.
						
						if (info12)
						{
							CP_XML_ATTR(L"unbalanced", info12->fUnbalancedReal);//??
							if (info12->fHidden)
								CP_XML_ATTR(L"hidden", info12->fHidden);
						}
					}

					if (sxTH->cisxvd > 0)
					{
						CP_XML_NODE(L"fieldsUsage")
						{
							CP_XML_ATTR(L"count", sxTH->cisxvd);
							for (size_t i = 0; i < sxTH->rgisxvd.size(); i++)
							{
								CP_XML_NODE(L"fieldUsage")
								{
									CP_XML_ATTR(L"x", sxTH->rgisxvd[i]);
								}
							}
						}
					}
				}

			}
		}
		CP_XML_NODE(L"kpis")
		{
			CP_XML_ATTR(L"count", mapKpis.size());
			
			for (std::unordered_map<std::wstring, int>::iterator it = mapKpis.begin(); it != mapKpis.end(); ++it)
			{
				SXAddl_SXCHierarchy_SXDKPIValue		*value	= NULL;
				SXAddl_SXCHierarchy_SXDKPIGoal		*goal	= NULL;
				SXAddl_SXCHierarchy_SXDKPIStatus	*status = NULL;
				SXAddl_SXCHierarchy_SXDKPITrend		*trend	= NULL;
				SXAddl_SXCHierarchy_SXDKPIWeight	*weight = NULL;
				SXAddl_SXCHierarchy_SXDKPITime		*time	= NULL;

				PIVOTADDL::_sxAddl & tmp = addls->m_arSXADDLHIERARCHY[it->second];

				for (size_t i = 0; bAddl && i < tmp.elements.size(); i++)
				{
					SXAddl * addl = dynamic_cast<SXAddl*>(tmp.elements[i].get());
					if (!addl) continue;

					if (!value)		value	= dynamic_cast<SXAddl_SXCHierarchy_SXDKPIValue*>	(addl->content.get());
					if (!goal)		goal	= dynamic_cast<SXAddl_SXCHierarchy_SXDKPIGoal*>		(addl->content.get());
					if (!status)	status	= dynamic_cast<SXAddl_SXCHierarchy_SXDKPIStatus*>	(addl->content.get());
					if (!trend)		trend	= dynamic_cast<SXAddl_SXCHierarchy_SXDKPITrend*>	(addl->content.get());
					if (!weight)	weight	= dynamic_cast<SXAddl_SXCHierarchy_SXDKPIWeight*>	(addl->content.get());
					if (!time)		time	= dynamic_cast<SXAddl_SXCHierarchy_SXDKPITime*>		(addl->content.get());
				}

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
			CP_XML_ATTR(L"count", mapDimensions.size() + (mapMeasures.empty() ? 0 : 1));

			if (!mapMeasures.empty())
			{
				CP_XML_NODE(L"dimension")
				{
					CP_XML_ATTR(L"measure",		L"1");
					CP_XML_ATTR(L"name",		L"Measures");
					CP_XML_ATTR(L"uniqueName",	L"[Measures]");
					CP_XML_ATTR(L"caption",		L"Measures");
				}
			}
			for (std::unordered_map<std::wstring, std::vector<int>>::iterator it = mapDimensions.begin(); it != mapDimensions.end(); ++it)
			{
				CP_XML_NODE(L"dimension")
				{
					CP_XML_ATTR(L"name",		it->first.substr(1, it->first.length() - 2));
					CP_XML_ATTR(L"uniqueName",	it->first);
					
					//if (sxTH->fMeasure)
					//	CP_XML_ATTR(L"measure",	true);
				}
			}
		}
		int count_maps = 0;
		CP_XML_NODE(L"measureGroups")
		{
			CP_XML_ATTR(L"count", mapMeasures.size());
			for (std::unordered_map<std::wstring, std::vector<int>>::iterator it = mapMeasures.begin(); it != mapMeasures.end(); ++it)
			{
				CP_XML_NODE(L"measureGroup")
				{
					CP_XML_ATTR(L"name", it->first);
					CP_XML_ATTR(L"caption", it->first);
				}
				//if (it->second.size() > 1)
				//	count_maps += mapDimensions;
				//else 
				//	count_maps += 1;				
				count_maps += it->second.size();
			}
		}
		CP_XML_NODE(L"maps")
		{
			int i = 0;
			CP_XML_ATTR(L"count", count_maps);
			for (std::unordered_map<std::wstring, std::vector<int>>::iterator it = mapMeasures.begin(); it != mapMeasures.end(); ++it, i++)
			{
				for (size_t j = 0; j < it->second.size(); j++)
				{
					CP_XML_NODE(L"map")
					{
						CP_XML_ATTR(L"measureGroup", i);
						
						PIVOTTH* th = dynamic_cast<PIVOTTH*>(m_arPIVOTTH[it->second[j]].get());
						SXTH* sxTH = dynamic_cast<SXTH*>(th->m_SXTH.get());
						
						CP_XML_ATTR(L"dimension", j + 1/*it->first*/);
					}
				}

			}
		}
	}
	return 0;
}
} // namespace XLS

