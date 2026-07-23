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
const bool SXFORMULA::saveContent(BinProcessor& proc)
{
	if(m_SXFMLA == nullptr)
		return false;
	proc.mandatory(*m_SXFMLA);
	if(m_PIVOTRULE != nullptr)
		proc.mandatory(*m_PIVOTRULE);
	if(m_SXFormula != nullptr)
		proc.mandatory(*m_SXFormula);
	return true;
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

