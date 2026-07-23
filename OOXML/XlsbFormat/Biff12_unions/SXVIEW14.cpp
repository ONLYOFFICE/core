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

#include "SXVIEW14.h"
#include "../Biff12_records/BeginSXView14.h"
#include "../Biff12_unions/SXCHANGES.h"
#include "../Biff12_unions/SXEDITS.h"
#include "../Biff12_unions/SXCONDFMTS14.h"
#include "../Biff12_records/EndSXView14.h"

using namespace XLS;

namespace XLSB
{

    SXVIEW14::SXVIEW14()
    {
    }

    SXVIEW14::~SXVIEW14()
    {
    }

    BaseObjectPtr SXVIEW14::clone()
    {
        return BaseObjectPtr(new SXVIEW14(*this));
    }

    //SXVIEW14 = BrtBeginSXView14 [SXCHANGES] [SXEDITS] [SXCONDFMTS14] BrtEndSXView14
    const bool SXVIEW14::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSXView14>())
        {
            m_BrtBeginSXView14 = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<SXCHANGES>())
        {
            m_SXCHANGES = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<SXEDITS>())
        {
            m_SXEDITS = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<SXCONDFMTS14>())
        {
            m_SXCONDFMTS14 = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<EndSXView14>())
        {
            m_bBrtEndSXView14 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSXView14 = false;

        return m_BrtBeginSXView14 && m_bBrtEndSXView14;
    }

	const bool SXVIEW14::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginSXView14 != nullptr)
			proc.mandatory(*m_BrtBeginSXView14);

		if (m_SXCHANGES != nullptr)
			proc.mandatory(*m_SXCHANGES);

		if (m_SXEDITS != nullptr)
			proc.mandatory(*m_SXEDITS);

		if (m_SXCONDFMTS14 != nullptr)
			proc.mandatory(*m_SXCONDFMTS14);

		proc.mandatory<EndSXView14>();

		return true;
	}

} // namespace XLSB

