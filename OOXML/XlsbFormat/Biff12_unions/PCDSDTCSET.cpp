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

#include "PCDSDTCSET.h"
#include "../Biff12_records/BeginPCDSDTCSet.h"
#include "../Biff12_unions/PCDSDTCEMEMBERS.h"
#include "../Biff12_unions/PCDSDTCEMEMBERSSORTBY.h"
#include "../Biff12_records/EndPCDSDTCSet.h"

using namespace XLS;

namespace XLSB
{

    PCDSDTCSET::PCDSDTCSET()
    {
    }

    PCDSDTCSET::~PCDSDTCSET()
    {
    }

    BaseObjectPtr PCDSDTCSET::clone()
    {
        return BaseObjectPtr(new PCDSDTCSET(*this));
    }

    //PCDSDTCSET = BrtBeginPCDSDTCSet *PCDSDTCEMEMBERS [PCDSDTCEMEMBERSSORTBY] BrtEndPCDSDTCSet
    const bool PCDSDTCSET::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDSDTCSet>())
        {
            m_BrtBeginPCDSDTCSet = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<PCDSDTCEMEMBERS>(0, 0);
        while(count > 0)
        {
            m_arPCDSDTCEMEMBERS.insert(m_arPCDSDTCEMEMBERS.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<PCDSDTCEMEMBERSSORTBY>())
        {
            m_PCDSDTCEMEMBERSSORTBY = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndPCDSDTCSet>())
        {
            m_bBrtEndPCDSDTCSet = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCDSDTCSet = false;

        return m_BrtBeginPCDSDTCSet && m_bBrtEndPCDSDTCSet;
    }

	const bool PCDSDTCSET::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPCDSDTCSet != nullptr)
			proc.mandatory(*m_BrtBeginPCDSDTCSet);

		for (auto &item : m_arPCDSDTCEMEMBERS)
		{
			proc.mandatory(*item);
		}

		if (m_PCDSDTCEMEMBERSSORTBY != nullptr)
			proc.mandatory(*m_PCDSDTCEMEMBERSSORTBY);

		proc.mandatory<EndPCDSDTCSet>();

		return true;
	}

} // namespace XLSB

