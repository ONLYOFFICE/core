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

#include "PIVOTVALUECELLS15.h"
#include "../Biff12_records/FRTBegin.h"
#include "../Biff12_records/BeginSxvcells.h"
#include "../Biff12_unions/PIVOTROWS15.h"
#include "../Biff12_records/EndSxvcells.h"
#include "../Biff12_records/FRTEnd.h"

using namespace XLS;

namespace XLSB
{

    PIVOTVALUECELLS15::PIVOTVALUECELLS15()
    {
    }

    PIVOTVALUECELLS15::~PIVOTVALUECELLS15()
    {
    }

    BaseObjectPtr PIVOTVALUECELLS15::clone()
    {
        return BaseObjectPtr(new PIVOTVALUECELLS15(*this));
    }

    // PIVOTVALUECELLS15 = BrtFRTBegin BrtBeginSxvcells 1*PIVOTROWS15 BrtEndSxvcells BrtFRTEnd
    const bool PIVOTVALUECELLS15::loadContent(BinProcessor& proc)
    {
        if (proc.optional<FRTBegin>())
        {
            m_BrtFRTBegin = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<BeginSxvcells>())
        {
            m_BrtBeginSxvcells = elements_.back();
            elements_.pop_back();
        }

        int count = proc.repeated<PIVOTROWS15>(0, 0);

        while(count > 0)
        {
            m_arPIVOTROWS15.insert(m_arPIVOTROWS15.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndSxvcells>())
        {
            m_bBrtEndSxvcells = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSxvcells = false;

        if (proc.optional<FRTEnd>())
        {
            m_bBrtFRTEnd = true;
            elements_.pop_back();
        }
		else
			m_bBrtFRTEnd = false;

        return m_BrtBeginSxvcells && m_bBrtEndSxvcells && m_bBrtFRTEnd;
    }

	const bool PIVOTVALUECELLS15::saveContent(BinProcessor& proc)
	{
		if (m_BrtFRTBegin != nullptr)
			proc.mandatory(*m_BrtFRTBegin);
		else
			proc.mandatory<FRTBegin>();

		if (m_BrtBeginSxvcells != nullptr)
			proc.mandatory(*m_BrtBeginSxvcells);

		for (auto& item : m_arPIVOTROWS15)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndSxvcells>();

		proc.mandatory<FRTEnd>();

		return true;
	}

} // namespace XLSB

