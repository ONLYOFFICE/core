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

	proc.mandatory<ATTACHEDLABEL>();

	m_ATTACHEDLABEL = elements_.back();
	elements_.pop_back();

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
	proc.mandatory<End>();				elements_.pop_back();
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

