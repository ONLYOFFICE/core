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

#include "BORDERS.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/BeginBorders.h"
#include "../Biff12_records/Border.h"
#include "../Biff12_records/EndBorders.h"

using namespace XLS;

namespace XLSB
{

    BORDERS::BORDERS()
    {
    }

    BORDERS::~BORDERS()
    {
    }

    BaseObjectPtr BORDERS::clone()
    {
        return BaseObjectPtr(new BORDERS(*this));
    }

    //BORDERS = BrtBeginBorders 1*65430BrtBorder BrtEndBorders
    const bool BORDERS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginBorders>())
        {
            m_BrtBeginBorders = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<Border>(1, 65430);
        while(count > 0)
        {
            m_arBrtBorder.insert(m_arBrtBorder.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

		if (proc.optional<EndBorders>())
		{
			m_bBrtEndBorders = true;
			elements_.pop_back();
		}
		else
			m_bBrtEndBorders = false;

        return m_BrtBeginBorders && !m_arBrtBorder.empty() && m_bBrtEndBorders;
    }

	const bool BORDERS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginBorders == nullptr)
			m_BrtBeginBorders = XLS::BaseObjectPtr(new XLSB::BeginBorders());

		if (m_BrtBeginBorders != nullptr)
		{
			auto ptrBrtBeginBorders = static_cast<XLSB::BeginBorders*>(m_BrtBeginBorders.get());

			if (ptrBrtBeginBorders != nullptr)
				ptrBrtBeginBorders->cborders = m_arBrtBorder.size();

			proc.mandatory(*m_BrtBeginBorders);
		}

		for (auto &item : m_arBrtBorder)
		{
			proc.mandatory(*item);
		}
		
		proc.mandatory<EndBorders>();

		return true;
	}

} // namespace XLSB

