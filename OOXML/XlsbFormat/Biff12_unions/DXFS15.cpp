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

#include "DXFS15.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/BeginDXFs15.h"
#include "../Biff12_records/FRTBegin.h"
#include "../Biff12_records/FRTEnd.h"
#include "../Biff12_records/EndDXFs15.h"

using namespace XLS;

namespace XLSB
{

    DXFS15::DXFS15()
    {
    }

    DXFS15::~DXFS15()
    {
    }

    BaseObjectPtr DXFS15::clone()
    {
        return BaseObjectPtr(new DXFS15(*this));
    }

    //DXFS15 = BrtFRTBegin BrtBeginDXFs15 1*2147483647BrtDXF14 BrtEndDXFs15 BrtFRTEnd
    const bool DXFS15::loadContent(BinProcessor& proc)
    {
        if (proc.optional<FRTBegin>())
        {
            m_BrtFRTBegin = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<BeginDXFs15>())
        {
            m_BrtBeginDXFs15 = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<DXF15>(0, 2147483647);
        while(count > 0)
        {
            m_arDXF15.insert(m_arDXF15.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndDXFs15>())
        {
            m_bBrtEndDXFs15 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndDXFs15 = false;

        if (proc.optional<FRTEnd>())
        {
            m_bBrtFRTEnd = true;
            elements_.pop_back();
        }
		else
			m_bBrtFRTEnd = false;

        return m_BrtBeginDXFs15 && !m_arDXF15.empty() && m_bBrtEndDXFs15;
    }

	const bool DXFS15::saveContent(BinProcessor& proc)
	{
		if (m_BrtFRTBegin != nullptr)
			proc.mandatory(*m_BrtFRTBegin);
		else
			proc.mandatory<FRTBegin>();

		if (m_BrtBeginDXFs15 == nullptr)
			m_BrtBeginDXFs15 = XLS::BaseObjectPtr(new XLSB::BeginDXFs15());

		if (m_BrtBeginDXFs15 != nullptr)
		{
			auto ptrBrtBeginDXFs15 = static_cast<XLSB::BeginDXFs15*>(m_BrtBeginDXFs15.get());

			if (ptrBrtBeginDXFs15 != nullptr)
				ptrBrtBeginDXFs15->cdxfs = m_arDXF15.size();

			proc.mandatory(*m_BrtBeginDXFs15);
		}

		proc.mandatory<EndDXFs15>();

		proc.mandatory<FRTEnd>();

		return true;
	}

} // namespace XLSB

