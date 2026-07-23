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

#include "MGS.h"
#include "../Biff12_records/BeginMGs.h"
#include "../Biff12_unions/MG.h"
#include "../Biff12_records/EndMGs.h"

using namespace XLS;

namespace XLSB
{

    MGS::MGS()
    {
    }

    MGS::~MGS()
    {
    }

    BaseObjectPtr MGS::clone()
    {
        return BaseObjectPtr(new MGS(*this));
    }

    //MGS = BrtBeginMGs *MG BrtEndMGs
    const bool MGS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginMGs>())
        {
            m_BrtBeginMGs = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<MG>(0, 0);
        while(count > 0)
        {
            m_arMG.insert(m_arMG.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndMGs>())
        {
            m_bBrtEndMGs = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndMGs = false;

        return m_BrtBeginMGs && m_bBrtEndMGs;
    }

	const bool MGS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginMGs == nullptr)
			m_BrtBeginMGs = XLS::BaseObjectPtr(new XLSB::BeginMGs());

		if (m_BrtBeginMGs != nullptr)
		{
			auto ptrBrtBeginMGs = static_cast<XLSB::BeginMGs*>(m_BrtBeginMGs.get());

			if (ptrBrtBeginMGs != nullptr)
				ptrBrtBeginMGs->cmgs = m_arMG.size();

			proc.mandatory(*m_BrtBeginMGs);
		}

		for (auto &item : m_arMG)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndMGs>();

		return true;
	}

} // namespace XLSB

