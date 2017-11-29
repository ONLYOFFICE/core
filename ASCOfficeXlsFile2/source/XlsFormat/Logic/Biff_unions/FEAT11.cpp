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

#include "FEAT11.h"
#include "SORTDATA12.h"

#include "../Biff_records/FeatHdr11.h"
#include "../Biff_records/Feature11.h"
#include "../Biff_records/Feature12.h"
#include "../Biff_records/ContinueFrt11.h"
#include "../Biff_records/List12.h"
#include "../Biff_records/AutoFilter12.h"
#include "../Biff_records/ContinueFrt12.h"
#include "../Biff_records/List12.h"

#include "../Biff_structures/List12BlockLevel.h"
#include "../Biff_structures/List12TableStyleClientInfo.h"
#include "../Biff_structures/List12DisplayName.h"
#include "../Biff_structures/Feat11FieldDataItem.h"

namespace XLS
{


FEAT11::FEAT11()
{
}


FEAT11::~FEAT11()
{
}

class Parenthesis_FEAT11_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_FEAT11_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_FEAT11_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.optional<Feature11>())
		{
			if(!proc.optional<Feature12>())
			{
				return false;
			}
		}
		int count = 0;
		count = proc.repeated<ContinueFrt11>(0, 0);
		count = proc.repeated<List12>(0, 0);
		
		while (proc.optional<AutoFilter12>())
		{
			count = proc.repeated<ContinueFrt12>(0, 0);
		}		
		
		count = proc.repeated<List12>(0, 0);
		proc.optional<SORTDATA12>();
		return true;
	};
};


BaseObjectPtr FEAT11::clone()
{
	return BaseObjectPtr(new FEAT11(*this));
}


/*
FEAT11 = FeatHdr11 *((Feature11 / Feature12) *ContinueFrt11 *List12 [AutoFilter12 *ContinueFrt12] *List12 [SORTDATA12])
*/
const bool FEAT11::loadContent(BinProcessor& proc)
{
	if (!proc.mandatory<FeatHdr11>())
	{
		return false;
	}
	m_FeatHdr11 = elements_.back();
	elements_.pop_back();
	
	int count = proc.repeated<Parenthesis_FEAT11_1>(0, 0);

	while(!elements_.empty())
	{
		switch(elements_.front()->get_type())
		{
			case typeFeature11: 
			case typeFeature12:
			{
				_data new_data;
				new_data.m_Feature = elements_.front();

				m_arFEAT.push_back(new_data);
			}break;
			case typeList12:
			{
				m_arFEAT.back().m_arList12.push_back(elements_.front());
			}break;
			case typeAutoFilter12:
			{
				m_arFEAT.back().m_AutoFilter12 = elements_.front();
			}break;
			case typeSORTDATA12:
			{
				m_arFEAT.back().m_SORTDATA12 = elements_.front();
			}break;
		}
		elements_.pop_front();
	}
	return true;
}

int FEAT11::serialize(std::wostream & strm, size_t index)
{
	FeatHdr11 * feature = dynamic_cast<FeatHdr11*>(m_FeatHdr11.get());

	Feature11 * feature11 = dynamic_cast<Feature11*>(m_arFEAT[index].m_Feature.get());
	Feature12 * feature12 = dynamic_cast<Feature12*>(m_arFEAT[index].m_Feature.get());
	
	List12BlockLevel			*block_level = NULL;
	List12TableStyleClientInfo	*table_style = NULL;
	List12DisplayName			*display_name = NULL;

	for (size_t i = 0; i < m_arFEAT[index].m_arList12.size(); i++)
	{
		List12* list_prop = dynamic_cast<List12*>(m_arFEAT[index].m_arList12[i].get());
		if (!list_prop) continue;

		if (!block_level)	block_level		= dynamic_cast<List12BlockLevel*>			(list_prop->rgbList12.get());
		if (!table_style)	table_style		= dynamic_cast<List12TableStyleClientInfo*>	(list_prop->rgbList12.get());
		if (!display_name)	display_name	= dynamic_cast<List12DisplayName*>			(list_prop->rgbList12.get());
	}
//----------------------------------------------------------------------------------------------------------------------------------
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"table")
		{
			CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");	
			if (display_name)
			{
				if (!display_name->stListName.value().empty())
					CP_XML_ATTR(L"displayName", display_name->stListName.value());	
				if (!display_name->stListComment.value().empty())
					CP_XML_ATTR(L"comment", display_name->stListComment.value());	
			}
			if (block_level)
			{
				if (!block_level->stData.value().empty())
					CP_XML_ATTR(L"dataCellStyle", block_level->stData.value());	
			}
			if (feature11)
			{
				CP_XML_ATTR(L"id", feature11->rgbFeat.idList);
				CP_XML_ATTR(L"name", feature11->rgbFeat.rgbName.value());
				CP_XML_ATTR(L"ref", feature11->sqref);

				if (feature11->rgbFeat.fAutoFilter)
				{
					CP_XML_NODE(L"autoFilter")
					{
						CP_XML_ATTR(L"ref", feature11->sqref);
					}
				}

				CP_XML_NODE(L"tableColumns")
				{
					CP_XML_ATTR(L"count", feature11->rgbFeat.arFieldData.size());
					for (size_t i = 0; i < feature11->rgbFeat.arFieldData.size(); i++)
					{
						Feat11FieldDataItem* field = dynamic_cast<Feat11FieldDataItem*>(feature11->rgbFeat.arFieldData[i].get());
						if(!field) continue;

						CP_XML_NODE(L"tableColumn")
						{
							CP_XML_ATTR(L"id", field->idField);
							CP_XML_ATTR(L"name", field->strCaption.value());

							if (field->dxfFmtAgg.bExist || 
								field->dxfFmtInsertRow.bExist)
							{
							//if (!field->stData.value().empty())
							//	CP_XML_ATTR(L"dataCellStyle", field->stData.value());	
							//if (!field->stData.value().empty())
							//	CP_XML_ATTR(L"dataDxfId", field->stData.value());	
							}
						}
					}
				}
			}
			else if (feature12)
			{
			}
			if (table_style)
			{
				CP_XML_NODE(L"tableStyleInfo")
				{
					CP_XML_ATTR(L"name", table_style->stListStyleName.value());	
					CP_XML_ATTR(L"showFirstColumn", table_style->nFirstColumn);	
					CP_XML_ATTR(L"showLastColumn", table_style->nLastColumn);	
					CP_XML_ATTR(L"showRowStripes", table_style->nRowStripes);	
					CP_XML_ATTR(L"showColumnStripes", table_style->nColumnStripes);	
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

