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

#include "PNAMES.h"
#include "../Biff12_records/BeginPNames.h"
#include "../Biff12_unions/PNAME.h"
#include "../Biff12_records/EndPNames.h"

using namespace XLS;

namespace XLSB
{

    PNAMES::PNAMES()
    {
    }

    PNAMES::~PNAMES()
    {
    }

    BaseObjectPtr PNAMES::clone()
    {
        return BaseObjectPtr(new PNAMES(*this));
    }

    //PNAMES = BrtBeginPNames *PNAME BrtEndPNames
    const bool PNAMES::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPNames>())
        {
            m_BrtBeginPNames = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<PNAME>(0, 0);
        while(count > 0)
        {
            m_arPNAME.insert(m_arPNAME.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndPNames>())
        {
            m_bBrtEndPNames = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPNames = false;

        return m_BrtBeginPNames && m_bBrtEndPNames;
    }

	const bool PNAMES::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPNames == nullptr)
			m_BrtBeginPNames = XLS::BaseObjectPtr(new XLSB::BeginPNames());

		if (m_BrtBeginPNames != nullptr)
		{
			auto ptrBrtBeginPNames = static_cast<XLSB::BeginPNames*>(m_BrtBeginPNames.get());

			if (ptrBrtBeginPNames != nullptr)
				ptrBrtBeginPNames->cnames = m_arPNAME.size();

			proc.mandatory(*m_BrtBeginPNames);
		}

		for (auto &item : m_arPNAME)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndPNames>();

		return true;
	}

} // namespace XLSB

