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

#include "PIVOTLI.h"

#include "../Biff_records/SXLI.h"
#include "../Biff_records/Continue.h"

namespace XLS
{

PIVOTLI::PIVOTLI(int count_lines_)
{
	count_lines = count_lines_;
}

PIVOTLI::~PIVOTLI()
{
}

BaseObjectPtr PIVOTLI::clone()
{
	return BaseObjectPtr(new PIVOTLI(*this));
}

// PIVOTLI = SXLI *Continue
const bool PIVOTLI::loadContent(BinProcessor& proc)
{
	SXLI sx_line(count_lines);

	if(!proc.mandatory(sx_line))
	{
		return false;
	}
	m_SXLI = elements_.back();
	elements_.pop_back();

	proc.repeated<Continue>(0, 0);

	return true;
}

int PIVOTLI::serialize(std::wostream & strm)
{
	SXLI* line_items = dynamic_cast<SXLI*>(m_SXLI.get());
	if (!line_items) return 0;

	CP_XML_WRITER(strm)
	{
 		for (size_t i = 0; i < line_items->m_arItems.size(); i++)
		{
			CP_XML_NODE(L"i")
			{
				switch(line_items->m_arItems[i].itmType)
				{
					case 0x0001:	CP_XML_ATTR(L"t", L"default");	break;
					case 0x0002:	CP_XML_ATTR(L"t", L"sum");		break;
					case 0x0003:	CP_XML_ATTR(L"t", L"countA");	break;
					case 0x0004:	CP_XML_ATTR(L"t", L"count");	break;
					case 0x0005:	CP_XML_ATTR(L"t", L"avg");		break;
					case 0x0006:	CP_XML_ATTR(L"t", L"max");		break;
					case 0x0007:	CP_XML_ATTR(L"t", L"min");		break;
					case 0x0008:	CP_XML_ATTR(L"t", L"product");	break;
					case 0x0009:	CP_XML_ATTR(L"t", L"stdDev");	break;
					case 0x000A:	CP_XML_ATTR(L"t", L"stdDevP");	break;
					case 0x000B:	CP_XML_ATTR(L"t", L"var");		break;
					case 0x000C:	CP_XML_ATTR(L"t", L"varP");		break;
					case 0x000D:	CP_XML_ATTR(L"t", L"grand");	break;
					case 0x000E:	CP_XML_ATTR(L"t", L"blank");	break;
				}

				if (line_items->m_arItems[i].cSic > 0 && line_items->m_arItems[i].itmType == 0)//??
				{
					CP_XML_ATTR(L"r", line_items->m_arItems[i].cSic);
				}

 				for (size_t j = line_items->m_arItems[i].cSic; j < line_items->m_arItems[i].rgisxvi.size(); j++)
				{
					CP_XML_NODE(L"x")
					{
						CP_XML_ATTR(L"v", line_items->m_arItems[i].rgisxvi[j]); 
					}
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

