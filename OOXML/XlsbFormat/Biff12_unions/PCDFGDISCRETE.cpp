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

#include "PCDFGDISCRETE.h"
#include "../Biff12_records/BeginPCDFGDiscrete.h"
#include "../Biff12_records/PCDIIndex.h"
#include "../Biff12_records/EndPCDFGDiscrete.h"

using namespace XLS;

namespace XLSB
{

    PCDFGDISCRETE::PCDFGDISCRETE()
    {
    }

    PCDFGDISCRETE::~PCDFGDISCRETE()
    {
    }

    BaseObjectPtr PCDFGDISCRETE::clone()
    {
        return BaseObjectPtr(new PCDFGDISCRETE(*this));
    }

    //PCDFGDISCRETE = BrtBeginPCDFGDiscrete 1*BrtPCDIIndex BrtEndPCDFGDiscrete
    const bool PCDFGDISCRETE::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDFGDiscrete>())
        {
            m_BrtBeginPCDFGDiscrete = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<PCDIIndex>(0, 0);
        while(count > 0)
        {
            m_arBrtPCDIIndex.insert(m_arBrtPCDIIndex.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndPCDFGDiscrete>())
        {
            m_bBrtEndPCDFGDiscrete = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCDFGDiscrete = false;

        return m_BrtBeginPCDFGDiscrete && m_bBrtEndPCDFGDiscrete;
    }

	const bool PCDFGDISCRETE::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPCDFGDiscrete == nullptr)
			m_BrtBeginPCDFGDiscrete = XLS::BaseObjectPtr(new XLSB::BeginPCDFGDiscrete());

		if (m_BrtBeginPCDFGDiscrete != nullptr)
		{
			auto ptrBrtBeginPCDFGDiscrete = static_cast<XLSB::BeginPCDFGDiscrete*>(m_BrtBeginPCDFGDiscrete.get());

			if (ptrBrtBeginPCDFGDiscrete != nullptr)
				ptrBrtBeginPCDFGDiscrete->cItems = m_arBrtPCDIIndex.size();

			proc.mandatory(*m_BrtBeginPCDFGDiscrete);
		}

		for (auto &item : m_arBrtPCDIIndex)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndPCDFGDiscrete>();

		return true;
	}

} // namespace XLSB

