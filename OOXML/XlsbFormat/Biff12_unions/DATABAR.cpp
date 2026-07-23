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

#include "DATABAR.h"
#include "../Biff12_records/BeginDatabar.h"
#include "../Biff12_unions/uCFVO.h"
#include "../Biff12_records/EndDatabar.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/Color.h"

using namespace XLS;

namespace XLSB
{

    DATABAR::DATABAR()
    {
    }

    DATABAR::~DATABAR()
    {
    }

    BaseObjectPtr DATABAR::clone()
    {
        return BaseObjectPtr(new DATABAR(*this));
    }

    // DATABAR = BrtBeginDatabar 2CFVO BrtColor BrtEndDatabar
    const bool DATABAR::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginDatabar>())
        {
            m_BrtBeginDatabar = elements_.back();
            elements_.pop_back();
        }
        else
            return false;

        int count = proc.repeated<uCFVO>(2, 2);

        while(count > 0)
        {
            m_arCFVO.insert(m_arCFVO.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<Color>())
        {
            m_BrtColor = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndDatabar>())
        {
			m_bBrtEndDatabar = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndDatabar = false;

        return m_BrtBeginDatabar && !m_arCFVO.empty() && m_BrtColor && m_bBrtEndDatabar;
    }

	const bool DATABAR::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginDatabar != nullptr)
			proc.mandatory(*m_BrtBeginDatabar);

		for (auto &item : m_arCFVO)
		{
			proc.mandatory(*item);
		}

		if (m_BrtColor != nullptr)
			proc.mandatory(*m_BrtColor);

		proc.mandatory<EndDatabar>();

		return true;
	}

} // namespace XLSB

