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

#include "TABLESTYLES.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_unions/TABLESTYLE.h"
#include "../Biff12_records/EndTableStyles.h"

using namespace XLS;

namespace XLSB
{

    TABLESTYLES::TABLESTYLES()
    {
    }

    TABLESTYLES::~TABLESTYLES()
    {
    }

    BaseObjectPtr TABLESTYLES::clone()
    {
        return BaseObjectPtr(new TABLESTYLES(*this));
    }

    //TABLESTYLES = BrtBeginTableStyles *TABLESTYLE BrtEndTableStyles
    const bool TABLESTYLES::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginTableStyles>())
        {
            m_BrtBeginTableStyles = elements_.back();
            elements_.pop_back();
        }

        int count = proc.repeated<TABLESTYLE>(0, 0);

        while(count > 0)
        {
            m_arTABLESTYLE.insert(m_arTABLESTYLE.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndTableStyles>())
        {
			m_bBrtEndTableStyles = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndTableStyles = false;

        return m_BrtBeginTableStyles && !m_arTABLESTYLE.empty() && m_bBrtEndTableStyles;
    }

	const bool TABLESTYLES::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginTableStyles != nullptr)
			proc.mandatory(*m_BrtBeginTableStyles);

		for (auto &item : m_arTABLESTYLE)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndTableStyles>();

		return true;
	}

} // namespace XLSB

