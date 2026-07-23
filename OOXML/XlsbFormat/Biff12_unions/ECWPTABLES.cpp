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

#include "ECWPTABLES.h"
#include "../Biff12_records/BeginEcWpTables.h"
#include "../Biff12_unions/PCDI.h"
#include "../Biff12_records/EndECWPTables.h"

using namespace XLS;

namespace XLSB
{

    ECWPTABLES::ECWPTABLES()
    {
    }

    ECWPTABLES::~ECWPTABLES()
    {
    }

    BaseObjectPtr ECWPTABLES::clone()
    {
        return BaseObjectPtr(new ECWPTABLES(*this));
    }

    //ECWPTABLES = BrtBeginEcWpTables *PCDI BrtEndECWPTabless
    const bool ECWPTABLES::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginEcWpTables>())
        {
            m_BrtBeginEcWpTables = elements_.back();
            elements_.pop_back();
        }

        int countPCDI = proc.repeated<PCDI>(0, 0);

        while(countPCDI > 0)
        {
            m_arPCDI.insert(m_arPCDI.begin(), elements_.back());
            elements_.pop_back();
            --countPCDI;
        }

        if (proc.optional<EndECWPTables>())
        {
            m_bBrtEndEcWpTables = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndEcWpTables = false;

        return m_BrtBeginEcWpTables && m_bBrtEndEcWpTables;
    }

	const bool ECWPTABLES::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginEcWpTables == nullptr)
			m_BrtBeginEcWpTables = XLS::BaseObjectPtr(new XLSB::BeginEcWpTables());

		if (m_BrtBeginEcWpTables != nullptr)
		{
			auto ptrBrtBeginEcWpTables = static_cast<XLSB::BeginEcWpTables*>(m_BrtBeginEcWpTables.get());

			if (ptrBrtBeginEcWpTables != nullptr)
				ptrBrtBeginEcWpTables->cTables = m_arPCDI.size();

			proc.mandatory(*m_BrtBeginEcWpTables);
		}

		for (auto &item : m_arPCDI)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndECWPTables>();

		return true;
	}

} // namespace XLSB

