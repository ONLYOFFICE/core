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

const bool PIVOTPI::saveContent(BinProcessor& proc)
{
    if(m_SXPI == nullptr)
        return false;
    proc.mandatory(*m_SXPI);
    auto castedPtr = static_cast<SXPI*>(m_SXPI.get());
    const auto maxRecSize = 8224;
    const auto continueElemSize = 6;
    while(!castedPtr->m_arItems.empty())
    {
        Continue continueRecord;
        if(castedPtr->m_arItems.size() < maxRecSize/continueElemSize)
            continueRecord.m_iDataSize = castedPtr->m_arItems.size() * continueElemSize;
        else
            continueRecord.m_iDataSize = maxRecSize;
        continueRecord.m_pData = new char[continueRecord.m_iDataSize];
        memcpy(continueRecord.m_pData, castedPtr->m_arItems.data(), continueRecord.m_iDataSize);
        proc.mandatory(continueRecord);
        castedPtr->m_arItems.erase(castedPtr->m_arItems.begin(),
            castedPtr->m_arItems.begin() + (continueRecord.m_iDataSize/continueElemSize));
    }
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

