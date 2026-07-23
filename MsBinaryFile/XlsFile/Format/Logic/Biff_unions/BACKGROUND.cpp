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

#include "BACKGROUND.h"
#include "../Biff_records/BkHim.h"
#include "../Biff_records/Continue.h"

namespace XLS
{


BACKGROUND::BACKGROUND()
{
}


BACKGROUND::~BACKGROUND()
{
}


BaseObjectPtr BACKGROUND::clone()
{
	return BaseObjectPtr(new BACKGROUND(*this));
}


// BACKGROUND = BkHim *Continue
const bool BACKGROUND::loadContent(BinProcessor& proc)
{
	pGlobalWorkbookInfoPtr = proc.getGlobalWorkbookInfo();

	if(!proc.mandatory<BkHim>())
	{
		return false;
	}
	m_BkHim = elements_.back();
	elements_.pop_back();

	int count = proc.repeated<Continue>(0, 0);

	return true;
}

const bool BACKGROUND::saveContent(BinProcessor& proc)
{
    if(m_BkHim == nullptr)
        return false;
    proc.mandatory(*m_BkHim);
    auto maxRecordSize = 8224;
    auto castedPtr = static_cast<BkHim*>(m_BkHim.get());
    if(castedPtr->lcb > maxRecordSize)
    {
        int remainedDataSize = castedPtr->lcb - maxRecordSize;
        size_t curDataPtr = maxRecordSize;
        while (remainedDataSize > 0)
        {
            Continue continueRecord;
            if(remainedDataSize < maxRecordSize)
            {
                continueRecord.m_iDataSize = remainedDataSize;
                continueRecord.m_pData = new char[remainedDataSize];
                memcpy(continueRecord.m_pData, (castedPtr->pData.get() + curDataPtr), remainedDataSize);
                proc.mandatory(continueRecord);
                break;
            }
            else
            {
                continueRecord.m_iDataSize = maxRecordSize;
                continueRecord.m_pData = new char[continueRecord.m_iDataSize];
                memcpy(continueRecord.m_pData, (castedPtr->pData.get() + curDataPtr), remainedDataSize);
                proc.mandatory(continueRecord);
                remainedDataSize -= maxRecordSize;
                curDataPtr += maxRecordSize;
            }
        }
    }
    return true;
}

int BACKGROUND::serialize(std::wostream & _stream)
{
	return 0;
}

} // namespace XLS

