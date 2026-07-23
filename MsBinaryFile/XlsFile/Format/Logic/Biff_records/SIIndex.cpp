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

#include "SIIndex.h"

#include "Number.h"
#include "BoolErr.h"
#include "Blank.h"
#include "Label.h"
#include "../../../../../OOXML/Base/Unit.h"


namespace XLS
{

SIIndex::SIIndex()
{
}


SIIndex::~SIIndex()
{
}


BaseObjectPtr SIIndex::clone()
{
	return BaseObjectPtr(new SIIndex(*this));
}


void SIIndex::readFields(CFRecord& record)
{
	record >> numIndex;
}

void SIIndex::writeFields(CFRecord& record)
{
    record << numIndex;
}

int SIIndex::serialize(std::wostream & _stream, int idx, const CellRef & in_ref)
{
	int res = 0;
	CP_XML_WRITER(_stream)    
	{	
		for (size_t i = 0 ; i < m_arData.size(); i++)
		{
			Number	* number	= dynamic_cast<Number*>	(m_arData[i].get());
			BoolErr * boolErr	= dynamic_cast<BoolErr*>(m_arData[i].get());
			Blank	* blank		= dynamic_cast<Blank *>	(m_arData[i].get());
			Label	* label		= dynamic_cast<Label *>	(m_arData[i].get());
			
			if (number)
			{
				if (in_ref == number->cell.getLocation())
				{
					CP_XML_NODE(L"c:pt")
					{
						CP_XML_ATTR(L"idx", idx++);
						CP_XML_NODE(L"c:v") { CP_XML_STREAM() << XmlUtils::ToString(number->num.data.value); }
					}
					res = 1;
				}
			}
			else if (label)
			{
				if (in_ref == label->cell.getLocation())
				{
					CP_XML_NODE(L"c:pt")
					{
						CP_XML_ATTR(L"idx", idx++);
						CP_XML_NODE(L"c:v") { CP_XML_STREAM() << XmlUtils::EncodeXmlString(label->st.value()); }
					}
					res = 1;
				}
			}
		}
	}
	return res;
}

int SIIndex::serialize(std::wostream & _stream, ChartParsedFormula & in_ref)
{
	int idx = 0;
	CP_XML_WRITER(_stream)    
	{	
		for (size_t i = 0 ; i < m_arData.size(); i++)
		{
			Number	* number	= dynamic_cast<Number*>	(m_arData[i].get());
			BoolErr * boolErr	= dynamic_cast<BoolErr*>(m_arData[i].get());
			Blank	* blank		= dynamic_cast<Blank *>	(m_arData[i].get());
			Label	* label		= dynamic_cast<Label *>	(m_arData[i].get());
			
			if (number)
			{
				if (in_ref.inRange(number->cell.getLocation()))
				{
					CP_XML_NODE(L"c:pt")
					{
						CP_XML_ATTR(L"idx", idx++);
						CP_XML_NODE(L"c:v") { CP_XML_STREAM() << std::to_wstring(number->num.data.value); }
					}
				}
			}
			else if (label)
			{
				if (in_ref.inRange(label->cell.getLocation()))
				{
					CP_XML_NODE(L"c:pt")
					{
						CP_XML_ATTR(L"idx", idx++);
						CP_XML_NODE(L"c:v") { CP_XML_STREAM() << XmlUtils::EncodeXmlString(label->st.value()); }
					}
				}
			}
			else
			{//??
				CP_XML_NODE(L"c:pt")
				{
					CP_XML_ATTR(L"idx", idx++);
					m_arData[i]->serialize( CP_XML_STREAM());
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

