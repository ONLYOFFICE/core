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

#include "LISTCOL.h"
#include "../Biff12_unions/ACUID.h"
#include "../Biff12_records/BeginListCol.h"
#include "../Biff12_records/ListCCFmla.h"
#include "../Biff12_records/ListTrFmla.h"
#include "../Biff12_unions/LISTXMLCPR.h"
#include "../Biff12_unions/FRT.h"
#include "../Biff12_records/EndListCol.h"

using namespace XLS;

namespace XLSB
{

    LISTCOL::LISTCOL()
    {
    }

    LISTCOL::~LISTCOL()
    {
    }

    BaseObjectPtr LISTCOL::clone()
    {
        return BaseObjectPtr(new LISTCOL(*this));
    }

    // LISTCOL = [ACUID] BrtBeginListCol [BrtListCCFmla] [BrtListTrFmla] [LISTXMLCPR] *FRT BrtEndListCol
    const bool LISTCOL::loadContent(BinProcessor& proc)
    {
        if (proc.optional<ACUID>())
        {
            //m_ACUID = elements_.back();
            elements_.pop_back();
        }

        BeginListCol beginlistcol;
        beginlistcol.indexList = indexList;
        if (proc.optional(beginlistcol))
        {
            m_BrtBeginListCol = elements_.back();
            elements_.pop_back();
        }        

        if (proc.optional<ListCCFmla>())
        {
            m_BrtListCCFmla = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<ListTrFmla>())
        {
            m_BrtListTrFmla = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<LISTXMLCPR>())
        {
            m_LISTXMLCPR = elements_.back();
            elements_.pop_back();
        }

        int count = proc.repeated<FRT>(0, 0);

        while(count > 0)
        {
            //m_arFRT.insert(m_arFRT.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndListCol>())
        {
            m_bBrtEndListCol = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndListCol = false;

        return m_BrtBeginListCol && m_bBrtEndListCol;
    }

	const bool LISTCOL::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginListCol != nullptr)
			proc.mandatory(*m_BrtBeginListCol);

		if (m_BrtListCCFmla != nullptr)
			proc.mandatory(*m_BrtListCCFmla);

		if (m_BrtListTrFmla != nullptr)
			proc.mandatory(*m_BrtListTrFmla);

		if (m_LISTXMLCPR != nullptr)
			proc.mandatory(*m_LISTXMLCPR);

		proc.mandatory<EndListCol>();

		return true;
	}

} // namespace XLSB

