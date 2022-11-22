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

#include "PIVOTPI.h"
#include "../Biff_records/SXPI.h"
#include "../Biff_records/Continue.h"

namespace XLS
{

PIVOTPI::PIVOTPI()
{
}


PIVOTPI::~PIVOTPI()
{
}


BaseObjectPtr PIVOTPI::clone()
{
	return BaseObjectPtr(new PIVOTPI(*this));
}

// PIVOTPI = SXPI *Continue
const bool PIVOTPI::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXPI>())
	{
		return false;
	}
	m_SXPI = elements_.back();
	elements_.pop_back();

	proc.repeated<Continue>(0, 0);

	return true;
}

int PIVOTPI::serialize(std::wostream & strm)
{
	SXPI* page_items = dynamic_cast<SXPI*>(m_SXPI.get());
	if (!page_items) return 0;

	CP_XML_WRITER(strm)
	{
 		for (size_t i = 0; i < page_items->m_arItems.size(); i++)
		{
			CP_XML_NODE(L"pageField")
			{
				CP_XML_ATTR(L"fld",	page_items->m_arItems[i].isxvd); 
				CP_XML_ATTR(L"hier",0); 
			}
		}
	}
	return 0;
}
} // namespace XLS

