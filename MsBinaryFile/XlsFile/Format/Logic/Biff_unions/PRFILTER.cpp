﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

