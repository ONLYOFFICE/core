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

#include "PCDCALCMEM15.h"
#include "../Biff12_records/FRTBegin.h"
#include "../Biff12_records/PCDCalcMem15.h"
#include "../Biff12_records/FRTEnd.h"

using namespace XLS;

namespace XLSB
{

    PCDCALCMEM15::PCDCALCMEM15()
    {
    }

    PCDCALCMEM15::~PCDCALCMEM15()
    {
    }

    BaseObjectPtr PCDCALCMEM15::clone()
    {
        return BaseObjectPtr(new PCDCALCMEM15(*this));
    }

    //PCDCALCMEM15 = BrtFRTBegin BrtPCDCalcMem15 BrtFRTEnd
    const bool PCDCALCMEM15::loadContent(BinProcessor& proc)
    {
        if (proc.optional<FRTBegin>())
        {
            m_BrtFRTBegin = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<PCDCalcMem15>())
        {
            m_BrtPCDCalcMem15 = elements_.back();
            elements_.pop_back();
        }
		if (proc.optional<FRTEnd>())
		{
			m_bBrtFRTEnd = true;
			elements_.pop_back();
		}
		else
			m_bBrtFRTEnd = false;

        return m_BrtFRTBegin && m_BrtPCDCalcMem15 && m_bBrtFRTEnd;
    }

	const bool PCDCALCMEM15::saveContent(BinProcessor& proc)
	{
		if (m_BrtFRTBegin != nullptr)
			proc.mandatory(*m_BrtFRTBegin);
		else
			proc.mandatory<FRTBegin>();

		if(m_BrtPCDCalcMem15 != nullptr)
			proc.mandatory(*m_BrtPCDCalcMem15);

		proc.mandatory<FRTEnd>();

		return true;
	}

} // namespace XLSB

