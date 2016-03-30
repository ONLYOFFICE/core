
#include "AUTOFILTER.h"
#include <Logic/Biff_records/AutoFilterInfo.h>
#include <Logic/Biff_records/AutoFilter12.h>
#include <Logic/Biff_records/AutoFilter.h>
#include <Logic/Biff_records/ContinueFrt12.h>
#include <Logic/Biff_unions/SORTDATA12.h>

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
	
	pGlobalWorkbookInfoPtr = proc.getGlobalWorkbookInfo();

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

					m_mapFilters12.insert(std::pair<int, std::vector<BaseObjectPtr>>(ind, ar));
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
	
	std::map<std::wstring, std::vector<std::wstring>>::iterator it = pGlobalWorkbookInfoPtr->mapDefineNames.find(L"_xlnm._FilterDatabase");
	
	if (it == pGlobalWorkbookInfoPtr->mapDefineNames.end()) return 0;

	int count_columns = info->cEntries;
	int ind = pGlobalWorkbookInfoPtr->current_sheet;
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

	std::wstring sheet_name = ind <= pGlobalWorkbookInfoPtr->sheets_names.size() ? pGlobalWorkbookInfoPtr->sheets_names[ind-1] : L"";
	if (!sheet_name.empty())
	{
		int pos = ref.find(sheet_name);
		if (pos >= 0)
		{
			pos = ref.find(L"!");
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
				
			for (int i = 0 ; i < m_arFilters.size(); i++)//todooo сделать на оставшиеся - count_columns (hidden)
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
							for (int j = 0 ; j < itF->second.size(); j++)
							{
								AutoFilter12* af12 = dynamic_cast<AutoFilter12*>(itF->second[j].get());
								if (af12 == NULL) continue;

								for (int k = 0 ; k < af12->rgbAF12Criteries.size(); k++)
								{
									AF12Criteria * af12Criteria = dynamic_cast<AF12Criteria *>(af12->rgbAF12Criteries[k].get());
									if (af12Criteria == NULL) continue;

									CP_XML_NODE(L"filter")
									{									
										CP_XML_ATTR(L"val", *af12Criteria->_str.value());
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

