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

#include "TABLE.h"
#include "../Biff12_unions/ACUID.h"
#include "../Biff12_records/BeginList.h"
#include "../Biff12_unions/AUTOFILTER.h"
#include "../Biff12_unions/SORTSTATE.h"
#include "../Biff12_unions/LISTCOLS.h"
#include "../Biff12_records/TableStyleClient.h"
#include "../Biff12_unions/FRTTABLE.h"
#include "../Biff12_records/EndList.h"

using namespace XLS;

namespace XLSB
{

    TABLE::TABLE()
    {
    }

    TABLE::~TABLE()
    {
    }

    BaseObjectPtr TABLE::clone()
    {
        return BaseObjectPtr(new TABLE(*this));
    }

    // TABLE = [ACUID] BrtBeginList [AUTOFILTER] [SORTSTATE] LISTCOLS BrtTableStyleClient FRTTABLE BrtEndList
    const bool TABLE::loadContent(BinProcessor& proc)
    {
        if (proc.optional<ACUID>())
        {
            //m_ACUID = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<BeginList>())
        {
            m_BrtBeginList = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<AUTOFILTER>())
        {
            m_AUTOFILTER = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<SORTSTATE>())
        {
            m_SORTSTATE = elements_.back();
            elements_.pop_back();
        }

        _UINT32 index = 0;
        if(m_BrtBeginList != nullptr)
            index = dynamic_cast<BeginList*>(m_BrtBeginList.get())->idList;

        LISTCOLS listcols;
        listcols.indexList = index;
        if (proc.optional(listcols))
        {
            m_LISTCOLS = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<TableStyleClient>())
        {
            m_BrtTableStyleClient = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<FRTTABLE>())
        {
            m_FRTTABLE = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndList>())
        {
            m_bBrtEndList = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndList = false;

        return m_BrtBeginList && m_LISTCOLS && m_BrtTableStyleClient && m_bBrtEndList;
    }

	const bool TABLE::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginList != nullptr)
			proc.mandatory(*m_BrtBeginList);

		if (m_AUTOFILTER != nullptr)
			proc.mandatory(*m_AUTOFILTER);

		if (m_SORTSTATE != nullptr)
			proc.mandatory(*m_SORTSTATE);

		if (m_LISTCOLS != nullptr)
			proc.mandatory(*m_LISTCOLS);

		if (m_BrtTableStyleClient != nullptr)
			proc.mandatory(*m_BrtTableStyleClient);

		if (m_FRTTABLE != nullptr)
			proc.mandatory(*m_FRTTABLE);

		proc.mandatory<EndList>();

		return true;
	}

} // namespace XLSB

