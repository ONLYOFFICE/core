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

#include "LD.h"
#include "ATTACHEDLABEL.h"
#include "FRAME.h"
#include "TEXTPROPS.h"
#include "CRTMLFRT.h"
#include "../Biff_records/Legend.h"
#include "../Biff_records/Begin.h"
#include "../Biff_records/Pos.h"
#include "../Biff_records/CrtLayout12.h"
#include "../Biff_records/FontX.h"
#include "../Biff_records/End.h"

namespace XLS
{


LD::LD()
{
	m_countSeries = 1;
}


LD::~LD()
{
}


BaseObjectPtr LD::clone()
{
	return BaseObjectPtr(new LD(*this));
}


// LD = Legend Begin Pos ATTACHEDLABEL [FRAME] [CrtLayout12] [TEXTPROPS] [CRTMLFRT] CRTMLFRT
const bool LD::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Legend>())
	{
		return false;
	}
	m_Legend = elements_.back();		elements_.pop_back();
	
	proc.mandatory<Begin>();			elements_.pop_back();

	if (proc.optional<Pos>())
	{
		m_Pos = elements_.back();
		elements_.pop_back();
	}

	if(proc.mandatory<ATTACHEDLABEL>())
	{
		m_ATTACHEDLABEL = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<FRAME>())
	{
		m_FRAME = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<CrtLayout12>())
	{
		m_CrtLayout12 = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<TEXTPROPS>())
	{
		m_TEXTPROPS = elements_.back();
		elements_.pop_back();
	}
	proc.optional<CRTMLFRT>();
	if(proc.mandatory<End>())
		elements_.pop_back();
	return true;
}

const bool LD::saveContent(BinProcessor& proc)
{
	if(m_Legend == nullptr)
		return false;
	proc.mandatory(*m_Legend);
	proc.mandatory<Begin>();
	if(m_Pos != nullptr)
		proc.mandatory(*m_Pos);
	if(m_ATTACHEDLABEL != nullptr)
		proc.mandatory(*m_ATTACHEDLABEL);
	if(m_FRAME != nullptr)
		proc.mandatory(*m_FRAME);
	if(m_CrtLayout12 != nullptr)
		proc.mandatory(*m_CrtLayout12);
	proc.mandatory<End>();
	return true;
}

int LD::serialize (std::wostream & _stream, const std::wstring & entries)
{
	ATTACHEDLABEL	*att	= dynamic_cast<ATTACHEDLABEL*>	(m_ATTACHEDLABEL.get());
	Legend			*legend = dynamic_cast<Legend *>		(m_Legend.get());
	
	if (!m_CrtLayout12 && !m_Legend)
		return 0;
	
	if (legend)
	{
		legend->m_CrtLayout12 = m_CrtLayout12;

		legend->serialize(_stream, m_countSeries);
	}else if (m_CrtLayout12)
		m_CrtLayout12->serialize(_stream);

	
	if (!entries.empty())
		_stream << entries;

	if (m_FRAME)
		m_FRAME->serialize(_stream);

	if (att)
		att->serialize_txPr(_stream);

	return 0;
}

} // namespace XLS

