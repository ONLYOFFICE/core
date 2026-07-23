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

#include "SXCONDFMT14.h"
#include "../Biff12_records/BeginSXCondFmt14.h"
#include "../Biff12_unions/PIVOTRULES14.h"
#include "../Biff12_records/EndSXCondFmt14.h"

using namespace XLS;

namespace XLSB
{

    SXCONDFMT14::SXCONDFMT14()
    {
    }

    SXCONDFMT14::~SXCONDFMT14()
    {
    }

    BaseObjectPtr SXCONDFMT14::clone()
    {
        return BaseObjectPtr(new SXCONDFMT14(*this));
    }

    //SXCONDFMT14 = BrtBeginSXCondFmt14 PIVOTRULES14 BrtEndSXCondFmt14
    const bool SXCONDFMT14::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSXCondFmt14>())
        {
            m_BrtBeginSXCondFmt14 = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PIVOTRULES14>())
        {
            m_PIVOTRULES14 = elements_.back();
            elements_.pop_back();
        }       

        if (proc.optional<EndSXCondFmt14>())
        {
            m_bBrtEndSXCondFmt14 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSXCondFmt14 = false;

        return m_BrtBeginSXCondFmt14 && m_bBrtEndSXCondFmt14;
    }

	const bool SXCONDFMT14::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginSXCondFmt14 != nullptr)
			proc.mandatory(*m_BrtBeginSXCondFmt14);

		if (m_PIVOTRULES14 != nullptr)
			proc.mandatory(*m_PIVOTRULES14);

		proc.mandatory<EndSXCondFmt14>();

		return true;
	}

} // namespace XLSB

