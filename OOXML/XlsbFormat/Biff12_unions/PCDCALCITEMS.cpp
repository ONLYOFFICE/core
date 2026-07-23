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

#include "PCDCALCITEMS.h"
#include "../Biff12_records/BeginPCDCalcItems.h"
#include "../Biff12_unions/PCDCALCITEM.h"
#include "../Biff12_records/EndPCDCalcItems.h"

using namespace XLS;

namespace XLSB
{

    PCDCALCITEMS::PCDCALCITEMS()
    {
    }

    PCDCALCITEMS::~PCDCALCITEMS()
    {
    }

    BaseObjectPtr PCDCALCITEMS::clone()
    {
        return BaseObjectPtr(new PCDCALCITEMS(*this));
    }

    //PCDCALCITEMS = BrtBeginPCDCalcItems 1*PCDCALCITEM BrtEndPCDCalcItems
    const bool PCDCALCITEMS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDCalcItems>())
        {
            m_BrtBeginPCDCalcItems = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<PCDCALCITEM>(0, 0);
        while(count > 0)
        {
            m_arPCDCALCITEM.insert(m_arPCDCALCITEM.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndPCDCalcItems>())
        {
            m_bBrtEndPCDCalcItems = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCDCalcItems = false;

        return m_BrtBeginPCDCalcItems && m_bBrtEndPCDCalcItems;
    }

	const bool PCDCALCITEMS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPCDCalcItems == nullptr)
			m_BrtBeginPCDCalcItems = XLS::BaseObjectPtr(new XLSB::BeginPCDCalcItems());

		if (m_BrtBeginPCDCalcItems != nullptr)
		{
			auto ptrBrtBeginPCDCalcItems = static_cast<XLSB::BeginPCDCalcItems*>(m_BrtBeginPCDCalcItems.get());

			if (ptrBrtBeginPCDCalcItems != nullptr)
				ptrBrtBeginPCDCalcItems->cItems = m_arPCDCALCITEM.size();

			proc.mandatory(*m_BrtBeginPCDCalcItems);
		}

		for (auto &item : m_arPCDCALCITEM)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndPCDCalcItems>();

		return true;
	}

} // namespace XLSB

