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

#include "SXFORMULA_bu.h"

#include "SXFMLA_bu.h"
#include "PIVOTRULE.h"
#include "PRFILTER.h"

#include "../Biff_records/SXFormula.h"
#include "../Biff_records/SxFmla.h"
#include "../Biff_records/SxName.h"
#include "../Biff_records/SXPair.h"
#include "../Biff_records/SxItm.h"
#include "../Biff_records/SxFilt.h"
#include "../Biff_records/SxRule.h"

namespace XLS
{

SXFORMULA::SXFORMULA()
{
}

SXFORMULA::~SXFORMULA()
{
}

BaseObjectPtr SXFORMULA::clone()
{
	return BaseObjectPtr(new SXFORMULA(*this));
}


// SXFORMULA = SXFMLA PIVOTRULE SXFormula
const bool SXFORMULA::loadContent(BinProcessor& proc)
{
	global_info = proc.getGlobalWorkbookInfo();

	if(!proc.mandatory<SXFMLA>())
	{
		return false;
	}
	m_SXFMLA = elements_.back();
	elements_.pop_back();

	if(proc.mandatory<PIVOTRULE>())
	{
		m_PIVOTRULE = elements_.back();
		elements_.pop_back();
	}
	if(proc.mandatory<SXFormula>())
	{
		m_SXFormula = elements_.back();
		elements_.pop_back();
	}	return true;
}
int SXFORMULA::serialize(std::wostream & strm)
{
	SXFMLA* fmla = dynamic_cast<SXFMLA*>(m_SXFMLA.get());
	if (!fmla) return 0;

	PIVOTRULE* pivot_rule = dynamic_cast<PIVOTRULE*>(m_PIVOTRULE.get());

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"calculatedItem")
		{
			fmla->serialize_attr(CP_GET_XML_NODE());

			if (pivot_rule)
			{
				SxRule* rule	= dynamic_cast<SxRule*>(pivot_rule->m_SxRule.get());

				for (size_t j = 0; j < pivot_rule->m_arPRFILTER.size(); j++)//multi in pivotAreas !!! todooo ???
				{
					PRFILTER*	filter	= dynamic_cast<PRFILTER*>(pivot_rule->m_arPRFILTER[j].get());
					
					SxItm*		item	= dynamic_cast<SxItm*>(filter->m_SxItm.get());
					SxFilt*		filt	= dynamic_cast<SxFilt*>(filter->m_SxFilt.get());
					
					CP_XML_NODE(L"pivotArea")
					{		
						CP_XML_ATTR(L"cacheIndex",		1);//true
						CP_XML_ATTR(L"outline",			0);
						CP_XML_ATTR(L"fieldPosition",	(int)rule->iDim);

						CP_XML_NODE(L"references")
						{		
							CP_XML_ATTR(L"count", item->rgisxvi.size());
							
							for (size_t i = 0; i < item->rgisxvi.size(); i++)
							{
								CP_XML_NODE(L"reference")
								{  								
									CP_XML_ATTR(L"field",	(int)filt->isxvd);
									CP_XML_ATTR(L"count",	(int)filt->iDim);

									CP_XML_NODE(L"x")
									{  
										CP_XML_ATTR(L"v", item->rgisxvi[i]);
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

