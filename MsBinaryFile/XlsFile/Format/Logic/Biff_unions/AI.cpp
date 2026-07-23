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

#include "AI.h"
#include "../Biff_records/BRAI.h"
#include "../Biff_records/SeriesText.h"
#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{


AI::AI()
{
}


AI::~AI()
{
}


BaseObjectPtr AI::clone()
{
	return BaseObjectPtr(new AI(*this));
}


// AI = BRAI [SeriesText]
const bool AI::loadContent(BinProcessor& proc)
{
	// fix
	if(!proc.optional<BRAI>())
	{
		return false;
	}
	m_BRAI = elements_.back();
	elements_.pop_back();

	if (proc.optional<SeriesText>())
	{
		m_SeriesText = elements_.back();
		elements_.pop_back();
	}

	return true;
}
const bool AI::saveContent(BinProcessor& proc)
{
	if(m_BRAI == nullptr)
		return false;
	proc.mandatory(*m_BRAI);
	if(m_SeriesText != nullptr)
		proc.mandatory(*m_SeriesText);
	return true;
}

bool AI::empty()
{
	if (!m_SeriesText && !m_BRAI) return true;

	BRAI* brai	= dynamic_cast<BRAI*>(m_BRAI.get());

	bool bEmpty = true;
	if (brai)		
	{
		std::wstring forumla = brai->formula.getAssembledFormula();		
		bEmpty = forumla.empty();
	}
	SeriesText * text = dynamic_cast<SeriesText *>(m_SeriesText.get());
	if (text && bEmpty)
	{
		bEmpty = text->stText.value().empty();
	}
	return bEmpty;
}

int AI::serialize(std::wostream & _stream)
{
	BRAI* brai	= dynamic_cast<BRAI*>(m_BRAI.get());

	if (brai)		
	{
		std::wstring forumla = brai->formula.getAssembledFormula();		
		if (forumla.empty() == false)
		{
			CP_XML_WRITER(_stream)    
			{
				CP_XML_NODE(L"c:tx")
				{
					CP_XML_NODE(L"c:strRef") 
					{
						CP_XML_NODE(L"c:f")
						{
							CP_XML_STREAM() << forumla;
						}
					}
				}
			}
			return 0;
		}
	}

	SeriesText * text = dynamic_cast<SeriesText *>(m_SeriesText.get());
	if ((text) && (text->stText.value().empty() == false))
	{
		CP_XML_WRITER(_stream)    
		{
			CP_XML_NODE(L"c:tx")
			{
				CP_XML_NODE(L"c:v") 
				{
					CP_XML_STREAM() << XmlUtils::EncodeXmlString(text->stText.value());
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

