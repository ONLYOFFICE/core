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

#include "LISTCOLS.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/BeginListCols.h"
#include "../Biff12_unions/LISTCOL.h"
#include "../Biff12_records/EndListCols.h"

using namespace XLS;

namespace XLSB
{

    LISTCOLS::LISTCOLS()
    {
    }

    LISTCOLS::~LISTCOLS()
    {
    }

    BaseObjectPtr LISTCOLS::clone()
    {
        return BaseObjectPtr(new LISTCOLS(*this));
    }

    //LISTCOLS = BrtBeginListCols 1*LISTCOL BrtEndListCols
    const bool LISTCOLS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginListCols>())
        {
            m_BrtBeginListCols = elements_.back();
            elements_.pop_back();
        }

        LISTCOL listcol;
        listcol.indexList = indexList;
        auto count = proc.repeated(listcol, 0, 0);
        while(count > 0)
        {
            m_arLISTCOL.insert(m_arLISTCOL.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

		if (proc.optional<EndListCols>())
		{
			m_bBrtEndListCols = true;
			elements_.pop_back();
		}
		else
			m_bBrtEndListCols = false;

        return m_BrtBeginListCols && !m_arLISTCOL.empty() && m_bBrtEndListCols;
    }

	const bool LISTCOLS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginListCols == nullptr)
			m_BrtBeginListCols = XLS::BaseObjectPtr(new XLSB::BeginListCols());

		if (m_BrtBeginListCols != nullptr)
		{
			auto ptrBeginListCols = static_cast<XLSB::BeginListCols*>(m_BrtBeginListCols.get());

			if (ptrBeginListCols != nullptr)
				ptrBeginListCols->nCols = m_arLISTCOL.size();

			proc.mandatory(*m_BrtBeginListCols);
		}

		for (auto &item : m_arLISTCOL)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndListCols>();

		return true;
	}

} // namespace XLSB

