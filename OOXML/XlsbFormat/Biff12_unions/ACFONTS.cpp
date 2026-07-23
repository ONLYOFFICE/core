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

#include "ACFONTS.h"
#include "../Biff12_records/ACBegin.h"
#include "../Biff12_records/KnownFonts.h"
#include "../Biff12_records/ACEnd.h"

using namespace XLS;

namespace XLSB
{

    ACFONTS::ACFONTS()
    {
    }

    ACFONTS::~ACFONTS()
    {
    }

    BaseObjectPtr ACFONTS::clone()
    {
        return BaseObjectPtr(new ACFONTS(*this));
    }

    //ACFONTS = BrtACBegin BrtKnownFonts BrtACEnd
    const bool ACFONTS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<ACBegin>())
        {
            m_BrtACBegin = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<KnownFonts>())
        {
            m_BrtKnownFonts = elements_.back();
            elements_.pop_back();
        }
		if (proc.optional<ACEnd>())
		{
			m_bBrtACEnd = true;
			elements_.pop_back();
		}
		else
			m_bBrtACEnd = false;

        return m_BrtACBegin && m_BrtKnownFonts && m_bBrtACEnd;
    }

	const bool ACFONTS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtACBegin != nullptr)
			proc.mandatory(*m_BrtACBegin);
		else
			proc.mandatory<ACBegin>();

		if (m_BrtKnownFonts != nullptr)
			proc.mandatory(*m_BrtKnownFonts);

		proc.mandatory<ACEnd>();

		return true;
	}

} // namespace XLSB

