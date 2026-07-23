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

#include "QSIR.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_unions/QSIFS.h"
#include "../Biff12_unions/DELETEDNAMES.h"
#include "../Biff12_unions/SORTSTATE.h"
#include "../Biff12_unions/FRT.h"
#include "../Biff12_records/EndQSIR.h"

using namespace XLS;

namespace XLSB
{

    QSIR::QSIR()
    {
    }

    QSIR::~QSIR()
    {
    }

    BaseObjectPtr QSIR::clone()
    {
        return BaseObjectPtr(new QSIR(*this));
    }

    // QSIR = BrtBeginQSIR QSIFS [DELETEDNAMES] [SORTSTATE] *FRT BrtEndQSIR
    const bool QSIR::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginQSIR>())
        {
            m_BrtBeginQSIR = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<QSIFS>())
        {
            m_QSIFS = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<DELETEDNAMES>())
        {
            m_DELETEDNAMES = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<SORTSTATE>())
        {
            m_SORTSTATE = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<FRT>(0, 0);
        while(count > 0)
        {
            //m_arFRT.insert(m_arFRT.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

		if (proc.optional<EndQSIR>())
		{
			m_bBrtEndQSIR = true;
			elements_.pop_back();
		}
		else
			m_bBrtEndQSIR = false;

        return m_BrtBeginQSIR && m_QSIFS && m_bBrtEndQSIR;
    }

	const bool QSIR::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginQSIR != nullptr)
			proc.mandatory(*m_BrtBeginQSIR);

		if (m_QSIFS != nullptr)
			proc.mandatory(*m_QSIFS);

		if (m_DELETEDNAMES != nullptr)
			proc.mandatory(*m_DELETEDNAMES);

		if (m_SORTSTATE != nullptr)
			proc.mandatory(*m_SORTSTATE);

		proc.mandatory<EndQSIR>();

		return true;
	}

} // namespace XLSB

