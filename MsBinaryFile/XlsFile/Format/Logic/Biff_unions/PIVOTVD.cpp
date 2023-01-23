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
	}

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
				CP_XML_ATTR(L"numFmtId", vd_ex->ifmt != 44 ? vd_ex->ifmt : 0);
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

