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

#include "EXTERNNAME.h"
#include "../Biff12_records/SupNameStart.h"
#include "../Biff12_records/SupNameFmla.h"
#include "../Biff12_records/SupNameBits.h"
#include "../Biff12_records/SupNameEnd.h"

using namespace XLS;

namespace XLSB
{

    EXTERNNAME::EXTERNNAME(ExternalReferenceType type) : sbt(type)
    {
    }

    EXTERNNAME::~EXTERNNAME()
    {
    }

    BaseObjectPtr EXTERNNAME::clone()
    {
        return BaseObjectPtr(new EXTERNNAME(*this));
    }

    //EXTERNNAME = BrtSupNameStart BrtSupNameFmla BrtSupNameBits BrtSupNameEnd
    const bool EXTERNNAME::loadContent(BinProcessor& proc)
    {
        if (proc.optional<SupNameStart>())
        {
            m_BrtSupNameStart = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<SupNameFmla>())
        {
            m_BrtSupNameFmla = elements_.back();
            elements_.pop_back();
        }

        SupNameBits subNameBits(sbt);
        if (proc.optional(subNameBits))
        {
            m_BrtSupNameBits = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<SupNameEnd>())
        {
            m_bBrtSupNameEnd = true;
            elements_.pop_back();
        }
		else
			m_bBrtSupNameEnd = false;

        return m_BrtSupNameStart && m_BrtSupNameFmla && m_BrtSupNameBits && m_bBrtSupNameEnd;
    }

	const bool EXTERNNAME::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtSupNameStart != nullptr)
			proc.mandatory(*m_BrtSupNameStart);

		if (m_BrtSupNameFmla != nullptr)
			proc.mandatory(*m_BrtSupNameFmla);

		if (m_BrtSupNameBits != nullptr)
			proc.mandatory(*m_BrtSupNameBits);

		proc.mandatory<SupNameEnd>();

		return true;
	}

} // namespace XLSB

