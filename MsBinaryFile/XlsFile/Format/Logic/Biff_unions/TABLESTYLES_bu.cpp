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

#include "TABLESTYLES.h"

#include "../Biff_records/TableStyles.h"
#include "../Biff_records/TableStyle.h"
#include "../Biff_records/TableStyleElement.h"

namespace XLS
{


TABLESTYLES::TABLESTYLES()
{
}


TABLESTYLES::~TABLESTYLES()
{
}



class Parenthesis_TABLESTYLES_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_TABLESTYLES_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_TABLESTYLES_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<TableStyle>())
		{
			return false;
		}
		int count = proc.repeated<TableStyleElement>(0, 28);
		return true;
	};
};


BaseObjectPtr TABLESTYLES::clone()
{
	return BaseObjectPtr(new TABLESTYLES(*this));
}


// TABLESTYLES = TableStyles *(TableStyle *28TableStyleElement)

const bool TABLESTYLES::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<TableStyles>())
	{
		return false;
	}
	
	m_TableStyles = elements_.back();
	elements_.pop_back();
		
	int count = proc.repeated<Parenthesis_TABLESTYLES_1>(0, 0);
	while(!elements_.empty())
	{
		if (elements_.front()->get_type() == typeTableStyle)
		{
			_table_style new_style;
			new_style.style_ = elements_.front();
			m_arTableStyles.push_back(new_style);
		}
		else
		{
			if (m_arTableStyles.size() > 0)
				m_arTableStyles.back().elements_.push_back(elements_.front());
		}
		elements_.pop_front();
	}	
	return true;
}
const bool TABLESTYLES::saveContent(BinProcessor& proc)
{
    if(m_TableStyles != nullptr)
        proc.mandatory(*m_TableStyles);
    for(auto i: m_arTableStyles)
    {
        proc.mandatory(*i.style_);
        for(auto j:i.elements_)
            proc.mandatory(*j);
    }
    return true;
}

int TABLESTYLES::serialize(std::wostream & stream)
{
	TableStyles * styles = dynamic_cast<TableStyles*>(m_TableStyles.get());
	if (!styles) return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"tableStyles")
		{
			CP_XML_ATTR(L"count", m_arTableStyles.size());
			
            if (!styles->rgchDefPivotStyle.empty())
			{
                CP_XML_ATTR(L"defaultPivotStyle", styles->rgchDefPivotStyle);
			}
            if (!styles->rgchDefTableStyle.empty())
			{
                CP_XML_ATTR(L"defaultTableStyle", styles->rgchDefTableStyle);
			}

			for (size_t i = 0; i < m_arTableStyles.size(); i++)
			{
				TableStyle* style = dynamic_cast<TableStyle*>(m_arTableStyles[i].style_.get());
				if (!style) continue;

				CP_XML_NODE(L"tableStyle")
				{
					CP_XML_ATTR(L"count", m_arTableStyles[i].elements_.size());
                    CP_XML_ATTR(L"name", style->rgchName);
					CP_XML_ATTR(L"pivot", style->fIsPivot);
					
					for (size_t j = 0; j < m_arTableStyles[i].elements_.size(); j++)
					{
						TableStyleElement* elem = dynamic_cast<TableStyleElement*>(m_arTableStyles[i].elements_[j].get());
						if (!elem) continue;

						CP_XML_NODE(L"tableStyleElement")
						{
							CP_XML_ATTR(L"dxfId", elem->index);

							switch(elem->tseType)
							{
							case 0x00000000: CP_XML_ATTR(L"type", L"wholeTable");			break;
							case 0x00000001: CP_XML_ATTR(L"type", L"headerRow");			break;
							case 0x00000002: CP_XML_ATTR(L"type", L"totalRow");				break;
							case 0x00000003: CP_XML_ATTR(L"type", L"firstColumn");			break;
							case 0x00000004: CP_XML_ATTR(L"type", L"lastColumn");			break;
							case 0x00000005: CP_XML_ATTR(L"type", L"firstRowStripe");		break;
							case 0x00000006: CP_XML_ATTR(L"type", L"secondRowStripe");		break;
							case 0x00000007: CP_XML_ATTR(L"type", L"firstColumnStripe");	break;
							case 0x00000008: CP_XML_ATTR(L"type", L"secondColumnStripe");	break;
							case 0x00000009: CP_XML_ATTR(L"type", L"firstHeaderCell");		break;
							case 0x0000000a: CP_XML_ATTR(L"type", L"lastHeaderCell");		break;
							case 0x0000000b: CP_XML_ATTR(L"type", L"firstTotalCell");		break;
							case 0x0000000c: CP_XML_ATTR(L"type", L"lastTotalCell");		break;
							case 0x0000000d: CP_XML_ATTR(L"type", L"firstSubtotalColumn");	break;
							case 0x0000000e: CP_XML_ATTR(L"type", L"secondSubtotalColumn");	break;
							case 0x0000000f: CP_XML_ATTR(L"type", L"thirdSubtotalColumn");	break;
							case 0x00000010: CP_XML_ATTR(L"type", L"firstSubtotalRow");		break;
							case 0x00000011: CP_XML_ATTR(L"type", L"secondSubtotalRow");	break;
							case 0x00000012: CP_XML_ATTR(L"type", L"thirdSubtotalRow");		break;
							case 0x00000013: CP_XML_ATTR(L"type", L"blankRow");				break;
							case 0x00000014: CP_XML_ATTR(L"type", L"firstColumnSubheading");break;
							case 0x00000015: CP_XML_ATTR(L"type", L"secondColumnSubheading");break;
							case 0x00000016: CP_XML_ATTR(L"type", L"thirdColumnSubheading");break;
							case 0x00000017: CP_XML_ATTR(L"type", L"firstRowSubheading");	break;
							case 0x00000018: CP_XML_ATTR(L"type", L"secondRowSubheading");	break;
							case 0x00000019: CP_XML_ATTR(L"type", L"thirdRowSubheading");	break;
							case 0x0000001a: CP_XML_ATTR(L"type", L"pageFieldLabels");		break;
							case 0x0000001b: CP_XML_ATTR(L"type", L"pageFieldValues");		break;
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

