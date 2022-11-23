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

#include "AUTOFILTER.h"
#include "SORTDATA12.h"

#include "../Biff_records/AutoFilterInfo.h"
#include "../Biff_records/AutoFilter12.h"
#include "../Biff_records/AutoFilter.h"
#include "../Biff_records/ContinueFrt12.h"
#include "../Biff_structures/AF12Criteria.h"

namespace XLS
{

AUTOFILTER::AUTOFILTER()
{
}

AUTOFILTER::~AUTOFILTER()
{
}

class Parenthesis_AUTOFILTER_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_AUTOFILTER_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_AUTOFILTER_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if (!proc.optional<AutoFilter>())
		{
			if(!proc.mandatory<AutoFilter12>())
			{
				return false;
			}
		}
		return true;
	};
};


BaseObjectPtr AUTOFILTER::clone()
{
	return BaseObjectPtr(new AUTOFILTER(*this));
}

// AUTOFILTER = AutoFilterInfo *(AutoFilter / (AutoFilter12 *ContinueFrt12)) *SORTDATA12
const bool AUTOFILTER::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<AutoFilterInfo>()) 
	{
		return false;
	}
	
	global_info_ = proc.getGlobalWorkbookInfo();

	m_AutoFilterInfo = elements_.back();
	elements_.pop_back();

	int count = proc.repeated<Parenthesis_AUTOFILTER_1>(0, 0);
	while(count > 0)
	{
		if (elements_.back()->get_type() == typeAutoFilter)
		{
			m_arFilters.insert(m_arFilters.begin(), elements_.back());
		}
		else
		{
			AutoFilter12* af12 = dynamic_cast<AutoFilter12*>(elements_.back().get());
			if (af12)
			{
				int ind = af12->iEntry;
				std::map<int, std::vector<BaseObjectPtr>>::iterator it = m_mapFilters12.find(ind);
				if (it != m_mapFilters12.end())
				{
					it->second.push_back(elements_.back());
				}
				else
				{
					std::vector<BaseObjectPtr> ar;
					ar.push_back(elements_.back());

					m_mapFilters12.insert(std::make_pair(ind, ar));
				}
			}
		}
		elements_.pop_back();
		count--;
	}	
	
	count = proc.repeated<SORTDATA12>(0, 0);
	while(count > 0)
	{
		m_arSORTDATA12.insert(m_arSORTDATA12.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	return true;
}

int AUTOFILTER::serialize(std::wostream & stream)
{
	if (m_AutoFilterInfo == NULL) return 0;

	AutoFilterInfo *info = dynamic_cast<AutoFilterInfo*>(m_AutoFilterInfo.get());
	
	std::map<std::wstring, std::vector<std::wstring>>::iterator it = global_info_->mapDefineNames.find(L"_xlnm._FilterDatabase");
	
	if (it == global_info_->mapDefineNames.end()) return 0;

	int count_columns = info->cEntries;
	
	size_t ind = global_info_->current_sheet;
	std::wstring ref;
	
	if (ind < it->second.size() && ind >= 0)
	{
		ref = it->second[ind];	//from current worksheet
	}

	if (ref.empty())
	{
		ref = it->second[0]; //from workbook
	}
	if (ref.empty()) return 0;

	std::wstring sheet_name = ind <= global_info_->sheets_info.size() ? global_info_->sheets_info[ind-1].name : L"";
	if (!sheet_name.empty())
	{
		int pos = ref.find(sheet_name);
		if (pos >= 0)
		{
			pos = ref.rfind(L"!");
			if (pos > 0)
				ref.erase(0, pos + 1);
		}
	}
	sheet_name = L"#REF";
	{
		int pos = ref.find(sheet_name);
		if (pos >= 0)
		{
			pos = ref.find(L"!");
			if (pos >= 0)
				ref.erase(0, pos + 1);
		}
	}

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"autoFilter")
		{	
			CP_XML_ATTR(L"ref", ref);

			//if (m_arFilters.empty() && m_arSORTDATA12.empty())
			//{
			//	CP_XML_NODE(L"filterColumn")
			//	{
			//		CP_XML_ATTR(L"colId", 0);
			//		CP_XML_ATTR(L"showButton", 0);
			//	}
			//}
			//else
			{				
				for (size_t i = 0 ; i < m_arFilters.size(); i++)//todooo сделать на оставшиеся - count_columns (hidden)
				{
					AutoFilter * filter = dynamic_cast<AutoFilter*>(m_arFilters[i].get());
					CP_XML_NODE(L"filterColumn")
					{
						CP_XML_ATTR(L"colId", filter->iEntry);
						
						CP_XML_NODE(L"filters")
						{
							if (filter->fSimple1 && !filter->str1.empty())
							{
								CP_XML_NODE(L"filter")
								{
									CP_XML_ATTR(L"val", filter->str1);
								}
							}
							if (filter->fSimple2 && !filter->str2.empty())
							{
								CP_XML_NODE(L"filter")
								{
									CP_XML_ATTR(L"val", filter->str2);
								}
							}

							std::map<int, std::vector<BaseObjectPtr>>::iterator itF = m_mapFilters12.find(filter->iEntry);
							if (itF != m_mapFilters12.end())
							{
								for (size_t j = 0 ; j < itF->second.size(); j++)
								{
									AutoFilter12* af12 = dynamic_cast<AutoFilter12*>(itF->second[j].get());
									if (af12 == NULL) continue;

									for (size_t k = 0 ; k < af12->arAF12Criteries.size(); k++)
									{
										AF12Criteria * af12Criteria = dynamic_cast<AF12Criteria *>(af12->arAF12Criteries[k].get());
										
										if (af12Criteria == NULL) continue;
										if (af12Criteria->_str.empty()) continue;

										CP_XML_NODE(L"filter")
										{									
											CP_XML_ATTR(L"val", af12Criteria->_str);
										}
									}
								}
							}
						}
					}
				}
			}
			if (false == m_arSORTDATA12.empty())
			{
				m_arSORTDATA12[0]->serialize(CP_XML_STREAM());
			}
		}
	}
	return 0;
}

} // namespace XLS

