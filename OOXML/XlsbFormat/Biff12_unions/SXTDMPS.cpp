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

#include "SXTDMPS.h"
#include "../Biff12_records/BeginSXTDMPs.h"
#include "../Biff12_unions/SXTDMP.h"
#include "../Biff12_records/EndSXTDMPs.h"

using namespace XLS;

namespace XLSB
{

    SXTDMPS::SXTDMPS()
    {
    }

    SXTDMPS::~SXTDMPS()
    {
    }

    BaseObjectPtr SXTDMPS::clone()
    {
        return BaseObjectPtr(new SXTDMPS(*this));
    }

    //SXTDMPS = BrtBeginSXTDMPs *SXTDMP BrtEndSXTDMPs
    const bool SXTDMPS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSXTDMPs>())
        {
            m_BrtBeginSXTDMPs = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<SXTDMP>(0, 0);
        while(count > 0)
        {
            m_arSXTDMP.insert(m_arSXTDMP.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndSXTDMPs>())
        {
            m_bBrtEndSXTDMPs = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSXTDMPs = false;

        return m_BrtBeginSXTDMPs && m_bBrtEndSXTDMPs;
    }

	const bool SXTDMPS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginSXTDMPs == nullptr)
			m_BrtBeginSXTDMPs = XLS::BaseObjectPtr(new XLSB::BeginSXTDMPs());

		if (m_BrtBeginSXTDMPs != nullptr)
		{
			auto ptrBrtBeginSXTDMPs = static_cast<XLSB::BeginSXTDMPs*>(m_BrtBeginSXTDMPs.get());

			if (ptrBrtBeginSXTDMPs != nullptr)
				ptrBrtBeginSXTDMPs->csxtdmp = m_arSXTDMP.size();

			proc.mandatory(*m_BrtBeginSXTDMPs);
		}

		for (auto &item : m_arSXTDMP)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndSXTDMPs>();

		return true;
	}

} // namespace XLSB

