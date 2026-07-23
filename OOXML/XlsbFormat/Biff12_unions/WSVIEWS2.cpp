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

#include "WSVIEWS2.h"
#include "../Biff12_records/BeginWsViews.h"
#include "../Biff12_unions/WSVIEW2.h"
#include "../Biff12_unions/FRT.h"
#include "../Biff12_records/EndWsViews.h"

using namespace XLS;

namespace XLSB
{

    WSVIEWS2::WSVIEWS2()
    {
    }

    WSVIEWS2::~WSVIEWS2()
    {
    }

    BaseObjectPtr WSVIEWS2::clone()
    {
        return BaseObjectPtr(new WSVIEWS2(*this));
    }

    // WSVIEWS2 = BrtBeginWsViews 1*WSVIEW2 *FRT BrtEndWsViews
    const bool WSVIEWS2::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginWsViews>())
        {
			m_bBrtBeginWsViews = true;
            elements_.pop_back();
        }
		else
			m_bBrtBeginWsViews = false;

        int countWSVIEW2 = proc.repeated<WSVIEW2>(0, 0);

        while(countWSVIEW2 > 0)
        {
            m_arWSVIEW2.insert(m_arWSVIEW2.begin(), elements_.back());
            elements_.pop_back();
            countWSVIEW2--;
        }

        int count = proc.repeated<FRT>(0, 0);

        while(count > 0)
        {
            //m_arFRT.insert(m_arFRT.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndWsViews>())
        {
            m_bBrtEndWsViews = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndWsViews = false;

        return m_bBrtBeginWsViews && !m_arWSVIEW2.empty() && m_bBrtEndWsViews;
    }

	const bool WSVIEWS2::saveContent(BinProcessor& proc)
	{
		proc.mandatory<BeginWsViews>();

		for (auto &item : m_arWSVIEW2)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndWsViews>();

		return true;
	}

} // namespace XLSB

