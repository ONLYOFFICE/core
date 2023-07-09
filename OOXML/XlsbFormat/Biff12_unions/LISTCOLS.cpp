﻿/*
 * (c) Copyright Ascensio System SIA 2010-2021
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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

