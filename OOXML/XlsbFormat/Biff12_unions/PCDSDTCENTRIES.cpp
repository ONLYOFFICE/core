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

#include "PCDSDTCENTRIES.h"
#include "../Biff12_records/BeginPCDSDTCEntries.h"
#include "../Biff12_unions/PCDSDTCENTRY.h"
#include "../Biff12_records/EndPCDSDTCEntries.h"

using namespace XLS;

namespace XLSB
{

    PCDSDTCENTRIES::PCDSDTCENTRIES()
    {
    }

    PCDSDTCENTRIES::~PCDSDTCENTRIES()
    {
    }

    BaseObjectPtr PCDSDTCENTRIES::clone()
    {
        return BaseObjectPtr(new PCDSDTCENTRIES(*this));
    }

    //PCDSDTCENTRIES = BrtBeginPCDSDTCEntries *PCDSDTCENTRY BrtEndPCDSDTCEntries
    const bool PCDSDTCENTRIES::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDSDTCEntries>())
        {
            m_BrtBeginPCDSDTCEntries = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<PCDSDTCENTRY>(0, 0);
        while(count > 0)
        {
            m_arPCDSDTCENTRY.insert(m_arPCDSDTCENTRY.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndPCDSDTCEntries>())
        {
            m_bBrtEndPCDSDTCEntries = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCDSDTCEntries = false;

        return m_BrtBeginPCDSDTCEntries && m_bBrtEndPCDSDTCEntries;
    }

	const bool PCDSDTCENTRIES::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPCDSDTCEntries == nullptr)
			m_BrtBeginPCDSDTCEntries = XLS::BaseObjectPtr(new XLSB::BeginPCDSDTCEntries());

		if (m_BrtBeginPCDSDTCEntries != nullptr)
		{
			auto ptrBrtBeginPCDSDTCEntries = static_cast<XLSB::BeginPCDSDTCEntries*>(m_BrtBeginPCDSDTCEntries.get());

			if (ptrBrtBeginPCDSDTCEntries != nullptr)
				ptrBrtBeginPCDSDTCEntries->cEntries = m_arPCDSDTCENTRY.size();

			proc.mandatory(*m_BrtBeginPCDSDTCEntries);
		}

		for (auto &item : m_arPCDSDTCENTRY)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndPCDSDTCEntries>();

		return true;
	}

} // namespace XLSB

