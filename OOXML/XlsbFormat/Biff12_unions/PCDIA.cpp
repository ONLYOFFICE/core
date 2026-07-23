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

#include "PCDIA.h"
#include "../Biff12_records/PCDIAMissing.h"
#include "../Biff12_records/PCDIANumber.h"
#include "../Biff12_records/PCDIABoolean.h"
#include "../Biff12_records/PCDIAError.h"
#include "../Biff12_records/PCDIAString.h"
#include "../Biff12_records/PCDIADatetime.h"

using namespace XLS;

namespace XLSB
{

    PCDIA::PCDIA()
    {
    }

    PCDIA::~PCDIA()
    {
    }

    BaseObjectPtr PCDIA::clone()
    {
        return BaseObjectPtr(new PCDIA(*this));
    }

    // PCDIA = BrtPCDIAMissing / BrtPCDIANumber / BrtPCDIABoolean / BrtPCDIAError / BrtPCDIAString / BrtPCDIADatetime
    const bool PCDIA::loadContent(BinProcessor& proc)
    {
        if (!proc.optional<PCDIAMissing>())
            if (!proc.optional<PCDIANumber>())
                if (!proc.optional<PCDIABoolean>())
                    if (!proc.optional<PCDIAError>())
                        if (!proc.optional<PCDIAString>())
                            if (!proc.optional<PCDIADatetime>())
                                return false;

        m_source = elements_.back();
        elements_.pop_back();

        return m_source != nullptr;
    }

	const bool PCDIA::saveContent(XLS::BinProcessor & proc)
	{
		if (m_source != nullptr)
			proc.mandatory(*m_source);

		return true;
	}

} // namespace XLSB

