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

#include "DBTABLES15.h"
#include "../Biff12_records/BeginDbTables15.h"
#include "../Biff12_records/DbTable15.h"
#include "../Biff12_records/EndDbTables15.h"

using namespace XLS;

namespace XLSB
{

    DBTABLES15::DBTABLES15()
    {
    }

    DBTABLES15::~DBTABLES15()
    {
    }

    BaseObjectPtr DBTABLES15::clone()
    {
        return BaseObjectPtr(new DBTABLES15(*this));
    }

    //DBTABLES15 = BrtBeginDbTables15 1*BrtDbTable15 BrtEndDbTables15
    const bool DBTABLES15::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginDbTables15>())
        {
			m_bBrtBeginDbTables15 = true;
            elements_.pop_back();
        }
		else
			m_bBrtBeginDbTables15 = false;

        int countDbTable15 = proc.repeated<DbTable15>(0, 0);

        while(countDbTable15 > 0)
        {
            m_arBrtDbTable15.insert(m_arBrtDbTable15.begin(), elements_.back());
            elements_.pop_back();
            countDbTable15--;
        }

        if (proc.optional<EndDbTables15>())
        {
            m_bBrtEndDbTables15 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndDbTables15 = false;

        return m_bBrtBeginDbTables15 && m_bBrtEndDbTables15;
    }

	const bool DBTABLES15::saveContent(XLS::BinProcessor & proc)
	{
		proc.mandatory<BeginDbTables15>();

		for (auto &item : m_arBrtDbTable15)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndDbTables15>();

		return true;
	}

} // namespace XLSB

