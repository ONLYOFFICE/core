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

#include "DROPBAR.h"
#include "GELFRAME.h"
#include "SHAPEPROPS.h"

#include "../Biff_records/DropBar.h"
#include "../Biff_records/LineFormat.h"
#include "../Biff_records/AreaFormat.h"

#include "../Biff_records/Begin.h"
#include "../Biff_records/End.h"
#include "../Biff_records/StartObject.h"
#include "../Biff_records/EndObject.h"

namespace XLS
{


DROPBAR::DROPBAR()
{
}


DROPBAR::~DROPBAR()
{
}


BaseObjectPtr DROPBAR::clone()
{
	return BaseObjectPtr(new DROPBAR(*this));
}


// DROPBAR = DropBar Begin LineFormat AreaFormat [GELFRAME] [SHAPEPROPS] End
const bool DROPBAR::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<DropBar>())
	{
		return false;
	}
	m_DropBar = elements_.back();			elements_.pop_back();
	
	if (proc.mandatory<Begin>())			elements_.pop_back();
	
	if (proc.optional<LineFormat>())
	{
		m_LineFormat = elements_.back();	elements_.pop_back();
	}
	if (proc.optional<StartObject>())
	{
		elements_.pop_back();
		if (proc.mandatory<EndObject>())	elements_.pop_back();
	}	
	if (proc.optional<AreaFormat>())
	{
		m_AreaFormat = elements_.back();	elements_.pop_back();
	}
	if (proc.optional<GELFRAME>())
	{
		m_GELFRAME = elements_.back();		elements_.pop_back();
	}
	
	if (proc.optional<SHAPEPROPS>())
	{
		m_SHAPEPROPS = elements_.back();	elements_.pop_back();
	}
	if (proc.mandatory<End>())				elements_.pop_back();

	return true;
}

const bool DROPBAR::saveContent(BinProcessor& proc)
{
	if(m_DropBar == nullptr)
		return false;
	proc.mandatory(*m_DropBar);
	proc.mandatory<Begin>();
	if(m_LineFormat != nullptr)
		proc.mandatory(*m_LineFormat);
	if(m_AreaFormat != nullptr)
		proc.mandatory(*m_AreaFormat);
	if(m_GELFRAME != nullptr)
		proc.optional(*m_GELFRAME);
	if(m_SHAPEPROPS != nullptr)
		proc.mandatory(*m_SHAPEPROPS);
	proc.mandatory<End>();
	return true;
}

int DROPBAR::serialize(std::wostream & _stream)
{
	DropBar *dropBar = dynamic_cast<DropBar*>(m_DropBar.get());
	if (!dropBar) return 0;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:spPr")
		{
			if (m_GELFRAME)
				m_GELFRAME->serialize(CP_XML_STREAM());
			else if (m_AreaFormat)
				m_AreaFormat->serialize(CP_XML_STREAM());

			if (m_LineFormat)
				m_LineFormat->serialize(CP_XML_STREAM());
		}
	}
	return 0;
}

} // namespace XLS

