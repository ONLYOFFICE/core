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

#include "PNPAIRS.h"
#include "../Biff12_records/BeginPNPairs.h"
#include "../Biff12_unions/PNPAIR.h"
#include "../Biff12_records/EndPNPairs.h"

using namespace XLS;

namespace XLSB
{

    PNPAIRS::PNPAIRS()
    {
    }

    PNPAIRS::~PNPAIRS()
    {
    }

    BaseObjectPtr PNPAIRS::clone()
    {
        return BaseObjectPtr(new PNPAIRS(*this));
    }

    //PNPAIRS = BrtBeginPNPairs PNPAIR BrtEndPNPairs // possibly an error, and should be *PNPAIR
    const bool PNPAIRS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPNPairs>())
        {
            m_BrtBeginPNPairs = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<PNPAIR>(0, 0);
        while(count > 0)
        {
            m_arPNPAIR.insert(m_arPNPAIR.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndPNPairs>())
        {
			m_bBrtEndPNPairs = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPNPairs = false;

        return m_BrtBeginPNPairs && m_bBrtEndPNPairs;
    }

	const bool PNPAIRS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPNPairs == nullptr)
			m_BrtBeginPNPairs = XLS::BaseObjectPtr(new XLSB::BeginPNPairs());

		if (m_BrtBeginPNPairs != nullptr)
		{
			auto ptrBrtBeginPNPairs = static_cast<XLSB::BeginPNPairs*>(m_BrtBeginPNPairs.get());

			if (ptrBrtBeginPNPairs != nullptr)
				ptrBrtBeginPNPairs->cpairs = m_arPNPAIR.size();

			proc.mandatory(*m_BrtBeginPNPairs);
		}

		for (auto &item : m_arPNPAIR)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndPNPairs>();

		return true;
	}

} // namespace XLSB

