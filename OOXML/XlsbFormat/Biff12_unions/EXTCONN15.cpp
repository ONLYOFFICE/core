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

#include "EXTCONN15.h"
#include "../Biff12_records/BeginExtConn15.h"
#include "../Biff12_unions/OLEDBPR15.h"
#include "../Biff12_unions/DATAFEEDPR15.h"
#include "../Biff12_unions/TEXTPR15.h"
#include "../Biff12_records/RangePr15.h"
#include "../Biff12_records/EndExtConn15.h"

using namespace XLS;

namespace XLSB
{

    EXTCONN15::EXTCONN15()
    {
    }

    EXTCONN15::~EXTCONN15()
    {
    }

    BaseObjectPtr EXTCONN15::clone()
    {
        return BaseObjectPtr(new EXTCONN15(*this));
    }

    // EXTCONN15 = BrtBeginExtConn15 [OLEDBPR15 / DATAFEEDPR15 / TEXTPR15 / BrtRangePr15] BrtEndExtConn15
    const bool EXTCONN15::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginExtConn15>())
        {
            m_BrtBeginExtConn15 = elements_.back();
            elements_.pop_back();
        }

        bool flag = true;
        if(!proc.optional<OLEDBPR15>())
           if(!proc.optional<DATAFEEDPR15>())
               if(!proc.optional<TEXTPR15>())
                   if(!proc.optional<RangePr15>())
                       flag = false;

        if(flag)
        {
            m_source = elements_.back();
            elements_.pop_back();
        }

		if (proc.optional<EndExtConn15>())
		{
			m_bBrtEndExtConn15 = true;
			elements_.pop_back();
		}
		else
			m_bBrtEndExtConn15 = false;

        return m_BrtBeginExtConn15 && m_bBrtEndExtConn15;
    }

	const bool EXTCONN15::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginExtConn15 != nullptr)
			proc.mandatory(*m_BrtBeginExtConn15);

		if (m_source != nullptr)
			proc.mandatory(*m_source);

		proc.mandatory<EndExtConn15>();

		return true;
	}

} // namespace XLSB

