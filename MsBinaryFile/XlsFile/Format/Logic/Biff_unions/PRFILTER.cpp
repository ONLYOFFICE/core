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

#include "PRFILTER.h"
#include "../Biff_records/SxFilt.h"
#include "../Biff_records/SxItm.h"
#include "../Biff_records/Continue.h"

namespace XLS
{

PRFILTER::PRFILTER()
{
}

PRFILTER::~PRFILTER()
{
}

BaseObjectPtr PRFILTER::clone()
{
	return BaseObjectPtr(new PRFILTER(*this));
}

// PRFILTER = SxFilt [SxItm *Continue]
const bool PRFILTER::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SxFilt>())
	{
		return false;
	}
	m_SxFilt = elements_.back();
	elements_.pop_back();

	if(proc.optional<SxItm>())
	{
		m_SxItm = elements_.back();
		elements_.pop_back();
		
		int count = proc.repeated<Continue>(0, 0);
	}
	return true;
}

const bool PRFILTER::saveContent(BinProcessor& proc)
{
    if(m_SxFilt == nullptr)
        return false;
    proc.mandatory(*m_SxFilt);
    if(m_SxItm != nullptr)
    {
        proc.mandatory(*m_SxItm);
        auto castedPtr = static_cast<SxItm*>(m_SxItm.get());
        const auto maxRecSize = 8224;
        const auto continueElemSize = 2;
        while(!castedPtr->rgisxvi.empty())
        {
            Continue continueRecord;
            if(castedPtr->rgisxvi.size() < maxRecSize/continueElemSize)
                continueRecord.m_iDataSize = castedPtr->rgisxvi.size() * continueElemSize;
            else
                continueRecord.m_iDataSize = maxRecSize;
            continueRecord.m_pData = new char[continueRecord.m_iDataSize];
            memcpy(continueRecord.m_pData, castedPtr->rgisxvi.data(), continueRecord.m_iDataSize);
            proc.mandatory(continueRecord);
            castedPtr->rgisxvi.erase(castedPtr->rgisxvi.begin(),
                castedPtr->rgisxvi.begin() + (continueRecord.m_iDataSize/continueElemSize));
        }
    }
    return true;
}

} // namespace XLS

