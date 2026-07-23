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

#include "SXVDS.h"
#include "../Biff12_records/BeginSXVDs.h"
#include "../Biff12_unions/SXVD.h"
#include "../Biff12_records/EndSXVDs.h"

using namespace XLS;

namespace XLSB
{

    SXVDS::SXVDS()
    {
    }

    SXVDS::~SXVDS()
    {
    }

    BaseObjectPtr SXVDS::clone()
    {
        return BaseObjectPtr(new SXVDS(*this));
    }

    //SXVDS = BrtBeginSXVDs 1*SXVD BrtEndSXVDs
    const bool SXVDS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSXVDs>())
        {
            m_BrtBeginSXVDs = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<SXVD>(0, 0);
        while(count > 0)
        {
            m_arSXVD.insert(m_arSXVD.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndSXVDs>())
        {
            m_bBrtEndSXVDs = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSXVDs = false;

        return m_BrtBeginSXVDs && !m_arSXVD.empty() && m_bBrtEndSXVDs;
    }

	const bool SXVDS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginSXVDs == nullptr)
			m_BrtBeginSXVDs = XLS::BaseObjectPtr(new XLSB::BeginSXVDs());

		if (m_BrtBeginSXVDs != nullptr)
		{
			auto ptrBrtBeginSXVDs = static_cast<XLSB::BeginSXVDs*>(m_BrtBeginSXVDs.get());

			if (ptrBrtBeginSXVDs != nullptr)
				ptrBrtBeginSXVDs->csxvds = m_arSXVD.size();

			proc.mandatory(*m_BrtBeginSXVDs);
		}

		for (auto &item : m_arSXVD)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndSXVDs>();

		return true;
	}

} // namespace XLSB

