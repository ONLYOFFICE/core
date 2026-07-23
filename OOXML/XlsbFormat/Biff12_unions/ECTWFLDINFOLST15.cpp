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

#include "ECTWFLDINFOLST15.h"
#include "../Biff12_records/BeginECTWFldInfoLst15.h"
#include "../Biff12_records/BeginECTwFldInfo15.h"
#include "../Biff12_records/EndECTWFldInfoLst15.h"

using namespace XLS;

namespace XLSB
{

    ECTWFLDINFOLST15::ECTWFLDINFOLST15()
    {
    }

    ECTWFLDINFOLST15::~ECTWFLDINFOLST15()
    {
    }

    BaseObjectPtr ECTWFLDINFOLST15::clone()
    {
        return BaseObjectPtr(new ECTWFLDINFOLST15(*this));
    }

    //ECTWFLDINFOLST15 = BrtBeginECTWFldInfoLst15 1*BrtBeginECTwFldInfo15 BrtEndECTWFldInfoLst15
    const bool ECTWFLDINFOLST15::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginECTWFldInfoLst15>())
        {
            m_BrtBeginECTWFldInfoLst15 = elements_.back();
            elements_.pop_back();
        }

        int countBeginECTwFldInfo15 = proc.repeated<BeginECTwFldInfo15>(0, 0);

        while(countBeginECTwFldInfo15 > 0)
        {
            m_arBrtBeginECTwFldInfo15.insert(m_arBrtBeginECTwFldInfo15.begin(), elements_.back());
            elements_.pop_back();
            countBeginECTwFldInfo15--;
        }

        if (proc.optional<EndECTWFldInfoLst15>())
        {
			m_bBrtEndECTWFldInfoLst15 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndECTWFldInfoLst15 = false;

        return m_BrtBeginECTWFldInfoLst15 && m_bBrtEndECTWFldInfoLst15;
    }

	const bool ECTWFLDINFOLST15::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginECTWFldInfoLst15 == nullptr)
			m_BrtBeginECTWFldInfoLst15 = XLS::BaseObjectPtr(new XLSB::BeginECTWFldInfoLst15());

		if (m_BrtBeginECTWFldInfoLst15 != nullptr)
		{
			auto ptrBrtBeginECTWFldInfoLst15 = static_cast<XLSB::BeginECTWFldInfoLst15*>(m_BrtBeginECTWFldInfoLst15.get());

			if (ptrBrtBeginECTWFldInfoLst15 != nullptr)
				ptrBrtBeginECTWFldInfoLst15->cFields = m_arBrtBeginECTwFldInfo15.size();

			proc.mandatory(*m_BrtBeginECTWFldInfoLst15);
		}

		for (auto &item : m_arBrtBeginECTwFldInfo15)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndECTWFldInfoLst15>();

		return true;
	}

} // namespace XLSB

