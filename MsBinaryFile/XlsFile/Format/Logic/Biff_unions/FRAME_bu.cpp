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

#include "FRAME.h"
#include "GELFRAME.h"
#include "SHAPEPROPS.h"
#include "../Biff_records/Frame.h"
#include "../Biff_records/Begin.h"
#include "../Biff_records/LineFormat.h"
#include "../Biff_records/AreaFormat.h"
#include "../Biff_records/End.h"
#include "../Biff_records/StartObject.h"
#include "../Biff_records/EndObject.h"

#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{


FRAME::FRAME()
{
}


FRAME::~FRAME()
{
}


BaseObjectPtr FRAME::clone()
{
	return BaseObjectPtr(new FRAME(*this));
}


// FRAME = Frame Begin LineFormat AreaFormat [GELFRAME] [SHAPEPROPS] End
const bool FRAME::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Frame>())
	{
		return false;
	}
	m_Frame = elements_.back();
	elements_.pop_back();

	if (proc.mandatory<Begin>())			elements_.pop_back();

	if (proc.mandatory<LineFormat>())
	{
		m_LineFormat = elements_.back();	elements_.pop_back();
	}
	if (proc.optional<StartObject>())
	{
		elements_.pop_back();
		if (proc.mandatory<EndObject>())	elements_.pop_back();
	}
	if (proc.mandatory<AreaFormat>())
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

	if(proc.mandatory<End>())				elements_.pop_back();

	return true;
}

int FRAME::serialize(std::wostream & _stream)
{
	AreaFormat * area = dynamic_cast<AreaFormat*>(m_AreaFormat.get());
	LineFormat * line = dynamic_cast<LineFormat*>(m_LineFormat.get());

	bool bArea = (m_GELFRAME && (area) && (area->fAuto == false)) ? true : false;
	if (!bArea && (area) && (area->fAuto == false)) bArea = true;

	bool bLine = false;
	if ((line) && (line->fAuto == false)) bLine = true;

	if (!bArea && !bLine) return 0;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:spPr")
		{
			if (bArea)
			{
				if (m_GELFRAME/* && area->fls > 1*/)
					m_GELFRAME->serialize(CP_XML_STREAM());
				else if (m_AreaFormat) 
					m_AreaFormat->serialize(CP_XML_STREAM());
			}
			
			if (m_LineFormat && bLine) m_LineFormat->serialize(CP_XML_STREAM());
		}
	}
	return 0;
}
} // namespace XLS

