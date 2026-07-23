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

#include "DVAL.h"
#include "../Biff_records/DVal.h"
#include "../Biff_records/Dv.h"

namespace XLS
{


DVAL::DVAL()
{
}


DVAL::~DVAL()
{
}


BaseObjectPtr DVAL::clone()
{
	return BaseObjectPtr(new DVAL(*this));
}


// DVAL = DVal *65534Dv
const bool DVAL::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<DVal>())
	{
		return false;
	}
	m_DVal = elements_.back();			elements_.pop_back();
	
	int count = proc.repeated<Dv>(0, 65534);
	
	while (count > 0)
	{
		m_arDv.insert(m_arDv.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}

	return true;
}

const bool DVAL::saveContent(BinProcessor& proc)
{
	if(m_DVal == nullptr)
		return false;
	proc.mandatory(*m_DVal);
	for(auto i : m_arDv)
		if(i != nullptr)
			proc.mandatory(*i);
	return true;
}

int DVAL::serialize(std::wostream & stream)
{
	if (!m_DVal)		return 0;
	if (m_arDv.empty()) return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"dataValidations")
		{
			CP_XML_ATTR(L"count", m_arDv.size());
			
			DVal * dval	= dynamic_cast<DVal*>(m_DVal.get());

			for (size_t i = 0 ; i < m_arDv.size(); i++)
			{
				if (!m_arDv[i]) continue;
				
				m_arDv[i]->serialize(CP_XML_STREAM());
			}
		}
	}

	return 0;
}

} // namespace XLS

