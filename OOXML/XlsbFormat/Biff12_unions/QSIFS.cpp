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

#include "QSIFS.h"
#include "../Biff12_records/BeginQSIFs.h"
#include "../Biff12_unions/QSIF.h"
#include "../Biff12_records/EndQSIFs.h"

using namespace XLS;

namespace XLSB
{

    QSIFS::QSIFS()
    {
    }

    QSIFS::~QSIFS()
    {
    }

    BaseObjectPtr QSIFS::clone()
    {
        return BaseObjectPtr(new QSIFS(*this));
    }

    // QSIFS = BrtBeginQSIFs *QSIF BrtEndQSIFs
    const bool QSIFS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginQSIFs>())
        {
            m_BrtBeginQSIFs = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<QSIF>(0, 0);
        while(count > 0)
        {
            m_arQSIF.insert(m_arQSIF.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndQSIFs>())
        {
            m_bBrtEndQSIFs = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndQSIFs = false;

        return m_BrtBeginQSIFs && m_bBrtEndQSIFs;
    }

	const bool QSIFS::saveContent(XLS::BinProcessor & proc)
	{		
		if (m_BrtBeginQSIFs == nullptr)
			m_BrtBeginQSIFs = XLS::BaseObjectPtr(new XLSB::BeginQSIFs());

		if (m_BrtBeginQSIFs != nullptr)
		{
			auto ptrBrtBeginQSIFs = static_cast<XLSB::BeginQSIFs*>(m_BrtBeginQSIFs.get());

			if (ptrBrtBeginQSIFs != nullptr)
				ptrBrtBeginQSIFs->nCols = m_arQSIF.size();

			proc.mandatory(*m_BrtBeginQSIFs);
		}

		for (auto &item : m_arQSIF)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndQSIFs>();

		return true;
	}

} // namespace XLSB

