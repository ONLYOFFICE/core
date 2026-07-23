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

#include "PCDCALCMEMSEXT.h"
#include "../Biff12_records/BeginPCDCalcMemsExt.h"
#include "../Biff12_unions/PCDCALCMEMEXT.h"
#include "../Biff12_records/EndPCDCalcMemsExt.h"

using namespace XLS;

namespace XLSB
{

    PCDCALCMEMSEXT::PCDCALCMEMSEXT()
    {
    }

    PCDCALCMEMSEXT::~PCDCALCMEMSEXT()
    {
    }

    BaseObjectPtr PCDCALCMEMSEXT::clone()
    {
        return BaseObjectPtr(new PCDCALCMEMSEXT(*this));
    }

    //PCDCALCMEMSEXT = BrtBeginPCDCalcMemsExt 1*2147483647PCDCALCMEMEXT BrtEndPCDCalcMemsExt
    const bool PCDCALCMEMSEXT::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDCalcMemsExt>())
        {
            m_BrtBeginPCDCalcMemsExt = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<PCDCALCMEMEXT>(0, 2147483647);
        while(count > 0)
        {
            m_arPCDCALCMEMEXT.insert(m_arPCDCALCMEMEXT.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndPCDCalcMemsExt>())
        {
            m_bBrtEndPCDCalcMemsExt = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCDCalcMemsExt = false;

        return m_BrtBeginPCDCalcMemsExt && !m_arPCDCALCMEMEXT.empty() && m_bBrtEndPCDCalcMemsExt;
    }

	const bool PCDCALCMEMSEXT::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPCDCalcMemsExt == nullptr)
			m_BrtBeginPCDCalcMemsExt = XLS::BaseObjectPtr(new XLSB::BeginPCDCalcMemsExt());

		if (m_BrtBeginPCDCalcMemsExt != nullptr)
		{
			auto ptrBrtBeginPCDCalcMemsExt = static_cast<XLSB::BeginPCDCalcMemsExt*>(m_BrtBeginPCDCalcMemsExt.get());

			if (ptrBrtBeginPCDCalcMemsExt != nullptr)
				ptrBrtBeginPCDCalcMemsExt->cCalcMems = m_arPCDCALCMEMEXT.size();

			proc.mandatory(*m_BrtBeginPCDCalcMemsExt);
		}

		for (auto &item : m_arPCDCALCMEMEXT)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndPCDCalcMemsExt>();

		return true;
	}

} // namespace XLSB

