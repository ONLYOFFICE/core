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

#include "PIVOTIVD.h"
#include "../Biff_records/SxIvd.h"
#include "../Biff_records/Continue.h"

namespace XLS
{

PIVOTIVD::PIVOTIVD()
{
}


PIVOTIVD::~PIVOTIVD()
{
}


BaseObjectPtr PIVOTIVD::clone()
{
	return BaseObjectPtr(new PIVOTIVD(*this));
}

// PIVOTIVD = SxIvd *Continue
const bool PIVOTIVD::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SxIvd>())
	{
		return false;
	}
	m_SxIvd = elements_.back();
	elements_.pop_back();

	int count = proc.repeated<Continue>(0, 0);

	return true;
}

const bool PIVOTIVD::saveContent(BinProcessor& proc)
{
    if(m_SxIvd == nullptr)
        return false;
    proc.mandatory(*m_SxIvd);
    auto castedPtr = static_cast<SxIvd*>(m_SxIvd.get());
    const auto maxRecSize = 8224;
    const auto continueElemSize = 2;
    while(!castedPtr->rgSxivd.empty())
    {
        Continue continueRecord;
        if(castedPtr->rgSxivd.size() < maxRecSize/continueElemSize)
            continueRecord.m_iDataSize = castedPtr->rgSxivd.size() * continueElemSize;
        else
            continueRecord.m_iDataSize = maxRecSize;
        continueRecord.m_pData = new char[continueRecord.m_iDataSize];
        memcpy(continueRecord.m_pData, castedPtr->rgSxivd.data(), continueRecord.m_iDataSize);
        proc.mandatory(continueRecord);
        castedPtr->rgSxivd.erase(castedPtr->rgSxivd.begin(),
            castedPtr->rgSxivd.begin() + (continueRecord.m_iDataSize/continueElemSize));
    }
    return true;
}

int PIVOTIVD::serialize(std::wostream & strm)
{
	SxIvd* vd = dynamic_cast<SxIvd*>(m_SxIvd.get());

	CP_XML_WRITER(strm)
	{
		for (size_t i = 0; i < vd->rgSxivd.size(); i++)
		{
			CP_XML_NODE(L"field")
			{ 
				CP_XML_ATTR(L"x", vd->rgSxivd[i]); 
			}
		}
	}
	return 0;
}
} // namespace XLS

