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

#include "PIVOTVD.h"
#include "../Biff_records/Sxvd.h"
#include "../Biff_records/SXVI.h"
#include "../Biff_records/SXVDEx.h"

namespace XLS
{

PIVOTVD::PIVOTVD()
{
}

PIVOTVD::~PIVOTVD()
{
}

BaseObjectPtr PIVOTVD::clone()
{
	return BaseObjectPtr(new PIVOTVD(*this));
}

// PIVOTVD = Sxvd *SXVI SXVDEx
const bool PIVOTVD::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Sxvd>())
	{
		return false;
	}
	GlobalWorkbookInfoPtr global_info = proc.getGlobalWorkbookInfo();

	m_Sxvd = elements_.back();
	elements_.pop_back();
	
	int count  = proc.repeated<SXVI>(0, 0);
	while(count--)
	{
		m_arSXVI.push_back(elements_.front());	elements_.pop_front();
	}

	if (proc.mandatory<SXVDEx>())
	{
		m_SXVDEx = elements_.back();
		elements_.pop_back();
		
		SXVDEx* vd_ex = dynamic_cast<SXVDEx*>(m_SXVDEx.get());
		if (vd_ex)
		{
			vd_ex->ifmt = global_info->RegisterNumFormat(vd_ex->ifmt, L""); // return update
		}
	}

	return true;
}
const bool PIVOTVD::saveContent(BinProcessor& proc)
{
    if(m_Sxvd == nullptr)
        return false;
    proc.mandatory(*m_Sxvd);
    for(auto i : m_arSXVI)
        if(i != nullptr)
            proc.mandatory(*i);
    if(m_SXVDEx != nullptr)
        proc.mandatory(*m_SXVDEx);
	else
		proc.mandatory<SXVDEx>();
    return true;
}

int PIVOTVD::serialize(std::wostream & strm)
{
	Sxvd*	vd		= dynamic_cast<Sxvd*>(m_Sxvd.get());
	SXVDEx* vd_ex	= dynamic_cast<SXVDEx*>(m_SXVDEx.get());

	if (!vd || !vd_ex) return 0;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"pivotField")
		{ 
			if		(vd->sxaxis.bRw)	CP_XML_ATTR(L"axis", L"axisRow"); 
			else if (vd->sxaxis.bCol)	CP_XML_ATTR(L"axis", L"axisCol"); 
			else if (vd->sxaxis.bPage)	CP_XML_ATTR(L"axis", L"axisPage"); 
			
			if (vd->sxaxis.bData)
			{
				CP_XML_ATTR(L"dataField", 1); 
			}
			
			CP_XML_ATTR(L"compact",	0); 
			
			if (vd_ex->ifmt > 0)	
			{
				CP_XML_ATTR(L"numFmtId", vd_ex->ifmt);
			}

			if (vd->stName.value().empty() == false)
				CP_XML_ATTR(L"name", vd->stName.value()); 

			if (vd->fCounta)	CP_XML_ATTR(L"countASubtotal",	1);
			if (vd->fCount)		CP_XML_ATTR(L"countSubtotal",	1);
			if (vd->fSum)		CP_XML_ATTR(L"sumSubtotal",		1);
			if (vd->fAverage)	CP_XML_ATTR(L"avgSubtotal",		1);
			if (vd->fMax)		CP_XML_ATTR(L"maxSubtotal",		1);
			if (vd->fMin)		CP_XML_ATTR(L"minSubtotal",		1);
			if (vd->fProduct)	CP_XML_ATTR(L"productSubtotal", 1);
			if (vd->fStdev)		CP_XML_ATTR(L"stdSubtotal",		1);
			if (vd->fStdevp)	CP_XML_ATTR(L"stdDevSubtotal",	1);
			if (vd->fVariance)	CP_XML_ATTR(L"varSubtotal",		1);
			if (vd->fVariancep)	CP_XML_ATTR(L"varPSubtotal",	1);

			CP_XML_ATTR(L"outline",			vd_ex->fOutline);
			CP_XML_ATTR(L"subtotalTop",		vd_ex->fSubtotalAtTop);
			CP_XML_ATTR(L"showAll",			vd_ex->fShowAllItems);
			
			if (vd_ex->fNotDragToData)		CP_XML_ATTR(L"dragToData",	0);
			if (!vd_ex->fDragToRow)			CP_XML_ATTR(L"dragToRow",	0);
			if (!vd_ex->fDragToColumn)		CP_XML_ATTR(L"dragToCol",	0);
			if (!vd_ex->fDragToPage)		CP_XML_ATTR(L"dragToPage",	0);
			
			if (vd_ex->fHideNewItems)		CP_XML_ATTR(L"hideNewItems",	1);
			if (vd_ex->fInsertBlankRow)		CP_XML_ATTR(L"insertBlankRow",	1);
			if (vd_ex->fPageBreaksBetweenItems)	CP_XML_ATTR(L"insertPageBreak", 1);
			if (!vd_ex->fTopAutoShow)		CP_XML_ATTR(L"topAutoShow",		0);
			//if (!vd_ex->fDragToHide)		CP_XML_ATTR(L"dragOff",	0);	//??
			if (vd_ex->fServerBased)		CP_XML_ATTR(L"serverField",		1);
			
			//CP_XML_ATTR(L"",				vd_ex->fCalculatedField);
			CP_XML_ATTR(L"includeNewItemsInFilter", 1); 

			//CP_XML_ATTR(L"nonAutoSortDefault", !vd_ex->fAutoSort);
			if (vd_ex->fAutoSort)
			{
			//	vd_ex->isxdiAutoSort
			
				if (vd_ex->fAscendSort)	CP_XML_ATTR(L"sortType", L"ascending"); 
			}
			if (vd_ex->isxdiAutoShow >= 0)
			{
				CP_XML_ATTR(L"rankBy", vd_ex->isxdiAutoShow);
			}		
			if (!vd->fDefault)	CP_XML_ATTR(L"defaultSubtotal",	0);
			if (!m_arSXVI.empty())
			{
				CP_XML_NODE(L"items")
				{
					CP_XML_ATTR(L"count", m_arSXVI.size()); 
					for (size_t i = 0; i < m_arSXVI.size(); i++)
					{
						m_arSXVI[i]->serialize(CP_XML_STREAM());
					}
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

