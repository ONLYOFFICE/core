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

#include "AXISPARENT.h"
#include "AXES.h"
#include "CRT.h"

#include "../Biff_records/AxisParent.h"
#include "../Biff_records/Begin.h"
#include "../Biff_records/Pos.h"
#include "../Biff_records/End.h"


namespace XLS
{


AXISPARENT::AXISPARENT()
{
}


AXISPARENT::~AXISPARENT()
{
}


BaseObjectPtr AXISPARENT::clone()
{
	return BaseObjectPtr(new AXISPARENT(*this));
}


// AXISPARENT = AxisParent Begin Pos [AXES] 1*4CRT End
const bool AXISPARENT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<AxisParent>())
	{
		return false;
	}
	m_AxisParent = elements_.back();
	elements_.pop_back();
	
	if (proc.optional<Begin>())		elements_.pop_back();

	if (proc.optional<Pos>())
	{
		m_Pos = elements_.back();
		elements_.pop_back(); 
	}

	int count = proc.repeated<AXES>(0, 0);
	if (count > 0)
	{
		m_AXES = elements_.back();
		elements_.pop_back(); 
	}
	
	count  = proc.repeated<CRT>(1, 4);

	while(count > 0 && !elements_.empty())
	{
		m_arCRT.insert(m_arCRT.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}	

	if (proc.optional<End>())	elements_.pop_back();

	return true;
}

const bool AXISPARENT::saveContent(BinProcessor& proc)
{
	if(m_AxisParent == nullptr)
		proc.mandatory<AxisParent>();
	else
		proc.mandatory(*m_AxisParent);
	proc.mandatory<Begin>();
	if(m_Pos != nullptr)
		proc.mandatory(*m_Pos);
	else
		proc.mandatory<Pos>();
	if(m_AXES != nullptr)
		proc.mandatory(*m_AXES);
	for(auto i: m_arCRT)
		if(i!= nullptr)
			proc.mandatory(*i);
	proc.mandatory<End>();
	return true;
}

void AXISPARENT::concatinate_second (BaseObjectPtr & addit)
{
	AXISPARENT * second = dynamic_cast<AXISPARENT *>(addit.get());

	if (second == NULL) return;

	for (size_t i = 0; i < second->m_arCRT.size(); i++)
	{
		CRT* crt = dynamic_cast<CRT*>(second->m_arCRT[i].get());
		
		if (crt)
			crt->m_indAXISPARENT = 1;

		m_arCRT.push_back(second->m_arCRT[i]);
	}

}


} // namespace XLS

