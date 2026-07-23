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

#include "FMD.h"
#include "FRT.h"
#include "DYNAMICARRAYMETADATA.h"
#include "RICHDATAMETADATA.h"

#include "../Biff12_records/BeginFmd.h"
#include "../Biff12_records/EndFmd.h"


using namespace XLS;

namespace XLSB
{

    FMD::FMD()
    {
    }

    FMD::~FMD()
    {
    }

    BaseObjectPtr FMD::clone()
    {
        return BaseObjectPtr(new FMD(*this));
    }

    //FMD = BrtBeginFmd COMMENTAUTHORS COMMENTLIST *FRT BrtEndFmd
    const bool FMD::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginFmd>())
        {
			m_BrtBeginFmd = true;
            elements_.pop_back();
        }
		else
			m_BrtBeginFmd = false;

       


        if (proc.optional<DYNAMICARRAYMETADATA>())
        {
            m_DYNAMICARRAYMETADATA = elements_.back();
            elements_.pop_back();
        }
        else if (proc.optional<RICHDATAMETADATA>())
        {
            m_RICHDATAMETADATA = elements_.back();
            elements_.pop_back();
        }
        else if(proc.optional<FRT>())
        {
            elements_.pop_back();
        }
        if (proc.optional<EndFmd>())
        {
            m_bBrtEndFmd = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndFmd = false;

        return m_BrtBeginFmd && m_bBrtEndFmd;
    }

	const bool FMD::saveContent(XLS::BinProcessor & proc)
	{
        proc.mandatory<BeginFmd>();
		if (m_DYNAMICARRAYMETADATA != nullptr)
			proc.mandatory(*m_DYNAMICARRAYMETADATA);
        else if(m_RICHDATAMETADATA != nullptr)
            proc.mandatory(*m_RICHDATAMETADATA);
        proc.mandatory<EndFmd>();
		return true;
	}

} // namespace XLSB

