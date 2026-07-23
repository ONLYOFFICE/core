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

#include "SXFMLA_bu.h"
#include "../Biff_records/SxFmla.h"
#include "../Biff_records/SxName.h"
#include "../Biff_records/SXPair.h"

namespace XLS
{

//  (SxName *SXPair)
class Parenthesis_SXFMLA: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXFMLA)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXFMLA(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SxName>())
		{
			return false;
		}	
		
		int count = proc.repeated<SXPair>(0, 0);

		return true;
	};
};

SXFMLA::SXFMLA()
{
}


SXFMLA::~SXFMLA()
{
}


BaseObjectPtr SXFMLA::clone()
{
	return BaseObjectPtr(new SXFMLA(*this));
}

// SXFMLA = SxFmla *(SxName *SXPair)
const bool SXFMLA::loadContent(BinProcessor& proc)
{
	global_info = proc.getGlobalWorkbookInfo();

	if(!proc.mandatory<SxFmla>())
	{
		return false;
	}
	m_SxFmla = elements_.back();
	elements_.pop_back();

	int count = proc.repeated<Parenthesis_SXFMLA>(0, 0);

	while(!elements_.empty())
	{
		if (dynamic_cast<SxName*>(elements_.front().get()))
		{
			_sx_name sx_name;
			sx_name.name = elements_.front(); elements_.pop_front();

			m_arPivotSxNames.push_back(sx_name);
		}
		else
		{
			while(!elements_.empty())
			{
				if (dynamic_cast<SxName*>(elements_.front().get()))
				{
					break;
				}
				else
				{
					m_arPivotSxNames.back().pair.push_back(elements_.front());
					elements_.pop_front();
				}
			}
		}
	}

	return true;
}

const bool SXFMLA::saveContent(BinProcessor& proc)
{
	if(m_SxFmla == nullptr)
		return false;
	proc.mandatory(*m_SxFmla);
	for(auto i : m_arPivotSxNames)
	{
		if(i.name != nullptr)
			proc.mandatory(*i.name);
		for(auto j : i.pair)
			if(j != nullptr)
				proc.mandatory(*j);
	}
	return true;
}

void SXFMLA::serialize_attr(CP_ATTR_NODE)
{
	if (!m_SxFmla) return;

	SxFmla* sx_fmla	= dynamic_cast<SxFmla*>(m_SxFmla.get()); 
	
	global_info->arPivotSxNames = m_arPivotSxNames;

	std::wstring fmla = sx_fmla->fmla.getAssembledFormula();
	CP_XML_ATTR(L"formula", fmla);
}


} // namespace XLS

