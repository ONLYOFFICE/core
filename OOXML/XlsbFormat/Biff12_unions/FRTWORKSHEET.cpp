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

#include "FRTWORKSHEET.h"
#include "../Biff12_unions/CONDITIONALFORMATTINGS.h"
#include "../Biff12_unions/DVALS14.h"
#include "../Biff12_unions/SPARKLINEGROUPS.h"
#include "../Biff12_unions/SLICERSEX.h"
#include "../Biff12_unions/TABLESLICERSEX.h"

using namespace XLS;

namespace XLSB
{

    FRTWORKSHEET::FRTWORKSHEET()
    {
    }

    FRTWORKSHEET::~FRTWORKSHEET()
    {
    }

    BaseObjectPtr FRTWORKSHEET::clone()
    {
        return BaseObjectPtr(new FRTWORKSHEET(*this));
    }

    // FRTWORKSHEET = [CONDITIONALFORMATTINGS] [DVALS14] [SPARKLINEGROUPS] [SLICERSEX]
    //                  [RANGEPROTECTION14] [IGNOREECS14] [WEBEXTENSIONS] [TABLESLICERSEX] [TIMELINESEX] *FRT
    const bool FRTWORKSHEET::loadContent(BinProcessor& proc)
    {
        if (proc.optional<CONDITIONALFORMATTINGS>())
        {
            m_CONDITIONALFORMATTINGS = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<DVALS14>())
        {
            m_DVALS14 = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<SPARKLINEGROUPS>())
        {
            m_SPARKLINEGROUPS = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<SLICERSEX>())
        {
            m_SLICERSEX = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<TABLESLICERSEX>())
        {
            m_TABLESLICERSEX = elements_.back();
            elements_.pop_back();
        }

        return m_CONDITIONALFORMATTINGS || m_DVALS14 || m_SPARKLINEGROUPS || m_SLICERSEX || m_TABLESLICERSEX;
    }

	const bool FRTWORKSHEET::saveContent(BinProcessor& proc)
	{
		if (m_CONDITIONALFORMATTINGS != nullptr)
			proc.mandatory(*m_CONDITIONALFORMATTINGS);

		if (m_DVALS14 != nullptr)
			proc.mandatory(*m_DVALS14);

		if (m_SPARKLINEGROUPS != nullptr)
			proc.mandatory(*m_SPARKLINEGROUPS);

		if (m_SLICERSEX != nullptr)
			proc.mandatory(*m_SLICERSEX);

		if (m_TABLESLICERSEX != nullptr)
			proc.mandatory(*m_TABLESLICERSEX);

		return true;
	}

} // namespace XLSB

