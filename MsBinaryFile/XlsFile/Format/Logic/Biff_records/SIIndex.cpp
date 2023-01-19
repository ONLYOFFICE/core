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

