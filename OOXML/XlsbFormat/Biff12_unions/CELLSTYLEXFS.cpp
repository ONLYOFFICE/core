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

#include "CELLSTYLEXFS.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/BeginCellStyleXFs.h"
#include "../Biff12_unions/FRT.h"
#include "../Biff12_records/EndCellStyleXFs.h"

using namespace XLS;

namespace XLSB
{

    CELLSTYLEXFS::CELLSTYLEXFS()
    {
    }

    CELLSTYLEXFS::~CELLSTYLEXFS()
    {
    }

    BaseObjectPtr CELLSTYLEXFS::clone()
    {
        return BaseObjectPtr(new CELLSTYLEXFS(*this));
    }

    //CELLSTYLEXFS = BrtBeginCellStyleXFs 1*65430(BrtXF *FRT) BrtEndCellStyleXFs
    const bool CELLSTYLEXFS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginCellStyleXFs>())
        {
            m_BrtBeginCellStyleXFs = elements_.back();
            elements_.pop_back();
        }

        size_t cell_xf_current_id = 0, style_xf_current_id = 0;
        XF xf(cell_xf_current_id, style_xf_current_id);
        while (proc.optional(xf))
        {
            m_arBrtXF.push_back(elements_.back());
            elements_.pop_back();

            while (proc.optional<FRT>())
            {
                //m_arFRT.push_back(elements_.back());
                elements_.pop_back();
            }
        }

        if (proc.optional<EndCellStyleXFs>())
        {
			m_bBrtEndCellStyleXFs = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndCellStyleXFs = false;

        return m_BrtBeginCellStyleXFs && !m_arBrtXF.empty() && m_bBrtEndCellStyleXFs;
    }

	const bool CELLSTYLEXFS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginCellStyleXFs == nullptr)
			m_BrtBeginCellStyleXFs = XLS::BaseObjectPtr(new XLSB::BeginCellStyleXFs());

		if (m_BrtBeginCellStyleXFs != nullptr)
		{
			auto ptrBrtBeginCellStyleXFs = static_cast<XLSB::BeginCellStyleXFs*>(m_BrtBeginCellStyleXFs.get());

			if (ptrBrtBeginCellStyleXFs != nullptr)
				ptrBrtBeginCellStyleXFs->cxfs = m_arBrtXF.size();

			proc.mandatory(*m_BrtBeginCellStyleXFs);
		}

		for (auto &item : m_arBrtXF)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndCellStyleXFs>();

		return true;
	}

} // namespace XLSB

