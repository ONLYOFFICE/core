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

