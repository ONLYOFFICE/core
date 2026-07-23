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

#include "FRTPCDCALCMEM.h"
#include "../Biff12_records/FRTBegin.h"
#include "../Biff12_unions/PCDCALCMEM14.h"
#include "../Biff12_records/FRTEnd.h"
#include "../Biff12_unions/FRT.h"

using namespace XLS;

namespace XLSB
{

    FRTPCDCALCMEM::FRTPCDCALCMEM()
    {
    }

    FRTPCDCALCMEM::~FRTPCDCALCMEM()
    {
    }

    BaseObjectPtr FRTPCDCALCMEM::clone()
    {
        return BaseObjectPtr(new FRTPCDCALCMEM(*this));
    }

    //FRTPCDCALCMEM = [BrtFRTBegin PCDCALCMEM14 BrtFRTEnd] *FRT
    const bool FRTPCDCALCMEM::loadContent(BinProcessor& proc)
    {
        if (proc.optional<FRTBegin>())
        {
            m_BrtFRTBegin = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PCDCALCMEM14>())
        {
            m_PCDCALCMEM14 = elements_.back();
            elements_.pop_back();
        }

		if (proc.optional<FRTEnd>())
		{
			m_bBrtFRTEnd = true;
			elements_.pop_back();
		}
		else
			m_bBrtFRTEnd = false;

        int count = proc.repeated<FRT>(0, 0);

        while(count > 0)
        {
            //m_arFRT.insert(m_arFRT.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }        

        return m_BrtFRTBegin && m_PCDCALCMEM14 && m_bBrtFRTEnd;
    }

	const bool FRTPCDCALCMEM::saveContent(BinProcessor& proc)
	{
		if (m_BrtFRTBegin != nullptr)
			proc.mandatory(*m_BrtFRTBegin);
		else
			proc.mandatory<FRTBegin>();

		if (m_PCDCALCMEM14 != nullptr)
			proc.mandatory(*m_PCDCALCMEM14);

		proc.mandatory<FRTEnd>();

		return true;
	}

} // namespace XLSB

