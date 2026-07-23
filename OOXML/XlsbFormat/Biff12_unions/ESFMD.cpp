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

#include "ESFMD.h"
#include "FMD.h"

#include "../Biff12_records/BeginEsfmd.h"
#include "../Biff12_records/EndEsfmd.h"
#include "../Biff12_unions/FRT.h"


using namespace XLS;

namespace XLSB
{

    ESFMD::ESFMD()
    {
    }

    ESFMD::~ESFMD()
    {
    }

    BaseObjectPtr ESFMD::clone()
    {
        return BaseObjectPtr(new ESFMD(*this));
    }

    //ESFMD = BrtBeginEsfmd COMMENTAUTHORS COMMENTLIST *FRT BrtEndEsfmd
    const bool ESFMD::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginEsfmd>())
        {
			m_BrtBeginEsfmd = elements_.back();
            elements_.pop_back();
        }
		else
			return false;

        int count = proc.repeated<FMD>(0, 0);

        while(count > 0)
        {
            FMDs.insert(FMDs.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        while (proc.optional<FRT>())
        {
            //m_arFRT.push_back(elements_.back());
            elements_.pop_back();
        }
        
        if (proc.optional<EndEsfmd>())
        {
            m_bBrtEndEsfmd = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndEsfmd = false;

        return m_BrtBeginEsfmd && m_bBrtEndEsfmd;
    }

	const bool ESFMD::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginEsfmd != nullptr)
			proc.mandatory(*m_BrtBeginEsfmd);
		for(auto i:FMDs)
            proc.mandatory(*i);
        proc.mandatory<XLSB::EndEsfmd>();
		return true;
	}

} // namespace XLSB

