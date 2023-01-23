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

