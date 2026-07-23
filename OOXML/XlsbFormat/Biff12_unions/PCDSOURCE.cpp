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

#include "PCDSOURCE.h"
#include "../Biff12_records/BeginPCDSource.h"
#include "../Biff12_unions/PCDSRANGE.h"
#include "../Biff12_unions/PCDSCONSOL.h"
#include "../Biff12_unions/PCDSOURCE14.h"
#include "../Biff12_records/EndPCDSource.h"

using namespace XLS;

namespace XLSB
{

    PCDSOURCE::PCDSOURCE()
    {
    }

    PCDSOURCE::~PCDSOURCE()
    {
    }

    BaseObjectPtr PCDSOURCE::clone()
    {
        return BaseObjectPtr(new PCDSOURCE(*this));
    }

    //PCDSOURCE = BrtBeginPCDSource [PCDSRANGE / PCDSCONSOL] PCDSOURCE14 BrtEndPCDSource
    const bool PCDSOURCE::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDSource>())
        {
            m_BrtBeginPCDSource = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<PCDSRANGE>())
        {
            m_PCDSRANGE = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<PCDSCONSOL>())
        {
            m_PCDSCONSOL = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<PCDSOURCE14>())
        {
            m_PCDSOURCE14 = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<EndPCDSource>())
        {
            m_bBrtEndPCDSource = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCDSource = false;

        return m_BrtBeginPCDSource && m_bBrtEndPCDSource;
    }

	const bool PCDSOURCE::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPCDSource != nullptr)
			proc.mandatory(*m_BrtBeginPCDSource);

		if (m_PCDSRANGE != nullptr)
			proc.mandatory(*m_PCDSRANGE);

		if (m_PCDSCONSOL != nullptr)
			proc.mandatory(*m_PCDSCONSOL);

		if (m_PCDSOURCE14 != nullptr)
			proc.mandatory(*m_PCDSOURCE14);

		proc.mandatory<EndPCDSource>();

		return true;
	}

} // namespace XLSB

