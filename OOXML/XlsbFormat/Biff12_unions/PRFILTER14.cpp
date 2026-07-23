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

#include "PRFILTER14.h"
#include "../Biff12_records/BeginPRFilter14.h"
#include "../Biff12_unions/PRFITEM14.h"
#include "../Biff12_records/EndPRFilter14.h"

using namespace XLS;

namespace XLSB
{

    PRFILTER14::PRFILTER14()
    {
    }

    PRFILTER14::~PRFILTER14()
    {
    }

    BaseObjectPtr PRFILTER14::clone()
    {
        return BaseObjectPtr(new PRFILTER14(*this));
    }

    //PRFILTER14 = BrtBeginPRFilter14 *PRFITEM14 BrtEndPRFilter14
    const bool PRFILTER14::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPRFilter14>())
        {
            m_BrtBeginPRFilter14 = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<PRFITEM14>(0, 0);
        while(count > 0)
        {
            m_arPRFITEM14.insert(m_arPRFITEM14.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndPRFilter14>())
        {
            m_bBrtEndPRFilter14 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPRFilter14 = false;

        return m_BrtBeginPRFilter14 && !m_arPRFITEM14.empty() && m_bBrtEndPRFilter14;
    }

	const bool PRFILTER14::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPRFilter14 != nullptr)
			proc.mandatory(*m_BrtBeginPRFilter14);

		for (auto &item : m_arPRFITEM14)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndPRFilter14>();

		return true;
	}

} // namespace XLSB

