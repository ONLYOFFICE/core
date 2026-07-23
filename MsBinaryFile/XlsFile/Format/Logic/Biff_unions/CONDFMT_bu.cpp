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

#include "CONDFMT.h"
#include "../Biff_records/CondFmt.h"
#include "../Biff_records/CF.h"

namespace XLS
{


CONDFMT::CONDFMT()
{
}


CONDFMT::~CONDFMT()
{
}


BaseObjectPtr CONDFMT::clone()
{
	return BaseObjectPtr(new CONDFMT(*this));
}


// CONDFMT = CondFmt 1*3CF
const bool CONDFMT::loadContent(BinProcessor& proc)
{
	CondFmt cond_fmt;
	if(!proc.mandatory(cond_fmt))
	{
		return false;
	}
	m_CondFmt = elements_.back();
	elements_.pop_back();

	while(true)
	{
		CF cf(cond_fmt.getLocation());
		if (proc.optional(cf))
		{
			m_arCF.push_back( elements_.back());
			elements_.pop_back();
		}
		else
			break;
	}

	return true;
}

const bool CONDFMT::saveContent(BinProcessor& proc)
{
    if(m_CondFmt != nullptr)
        proc.mandatory(*m_CondFmt);
    for(auto i : m_arCF)
        if(i != nullptr)
            proc.mandatory(*i);
    return true;
}
int CONDFMT::serialize(std::wostream & stream)
{
	if (!m_CondFmt)			return 0;
	if (m_arCF.size() < 1)	return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"conditionalFormatting")
		{
			CondFmt * condFmt	= dynamic_cast<CondFmt*>(m_CondFmt.get());

			CP_XML_ATTR(L"sqref", condFmt->sqref.strValue);
			//condition_id
			//condition_pos
			for (size_t i = 0; i < m_arCF.size(); i++)
			{
				if (m_arCF[i] == NULL) continue;

				m_arCF[i]->serialize(CP_XML_STREAM());
			}
		}
	}
	return 0;
}
} // namespace XLS

