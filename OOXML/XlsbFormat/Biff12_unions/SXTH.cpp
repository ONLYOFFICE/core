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

#include "SXTH.h"
#include "../Biff12_records/BeginSXTH.h"
#include "../Biff12_unions/SXTDMPS.h"
#include "../Biff12_unions/SXTHITEMS.h"
#include "../Biff12_unions/FRTSXTH.h"
#include "../Biff12_records/EndSXTH.h"

using namespace XLS;

namespace XLSB
{

    SXTH::SXTH()
    {
    }

    SXTH::~SXTH()
    {
    }

    BaseObjectPtr SXTH::clone()
    {
        return BaseObjectPtr(new SXTH(*this));
    }

    //SXTH = BrtBeginSXTH [SXTDMPS] *SXTHITEMS FRTSXTH BrtEndSXTH
    const bool SXTH::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSXTH>())
        {
            m_BrtBeginSXTH = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<SXTDMPS>())
        {
            m_SXTDMPS = elements_.back();
            elements_.pop_back();
        }
        auto count = proc.repeated<SXTHITEMS>(0, 0);
        while(count > 0)
        {
            m_arSXTHITEMS.insert(m_arSXTHITEMS.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }
        if (proc.optional<FRTSXTH>())
        {
            m_FRTSXTH = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<EndSXTH>())
        {
            m_bBrtEndSXTH = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSXTH = false;

        return m_BrtBeginSXTH && m_bBrtEndSXTH;
    }

	const bool SXTH::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginSXTH != nullptr)
			proc.mandatory(*m_BrtBeginSXTH);

		if (m_SXTDMPS != nullptr)
			proc.mandatory(*m_SXTDMPS);

		for (auto &item : m_arSXTHITEMS)
		{
			proc.mandatory(*item);
		}

		if (m_FRTSXTH != nullptr)
			proc.mandatory(*m_FRTSXTH);

		proc.mandatory<EndSXTH>();

		return true;
	}

} // namespace XLSB

