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

#include "COLORSCALE.h"

#include "ACFILTERS.h"
#include "../Biff12_records/BeginColorScale.h"
#include "../Biff12_unions/uCFVO.h"
#include "../Biff12_records/EndColorScale.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/Color.h"

using namespace XLS;

namespace XLSB
{

    COLORSCALE::COLORSCALE()
    {
    }

    COLORSCALE::~COLORSCALE()
    {
    }

    BaseObjectPtr COLORSCALE::clone()
    {
        return BaseObjectPtr(new COLORSCALE(*this));
    }

    // COLORSCALE = BrtBeginColorScale ((2CFVO 2BrtColor) / (3CFVO 3BrtColor)) BrtEndColorScale
    const bool COLORSCALE::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginColorScale>())
        {
            m_bBrtBeginColorScale = true;
            elements_.pop_back();
        }        
        else
            return false;

        int countCFVO = proc.repeated<uCFVO>(2, 3);

        while(countCFVO > 0)
        {
            m_arCFVO.insert(m_arCFVO.begin(), elements_.back());
            elements_.pop_back();
            countCFVO--;
        }

        int countColor = proc.repeated<Color>(2, 3);

        while(countColor > 0)
        {
            m_arBrtColor.insert(m_arBrtColor.begin(), elements_.back());
            elements_.pop_back();
            countColor--;
        }

        if (proc.optional<EndColorScale>())
        {
            m_bBrtEndColorScale = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndColorScale = false;

        return m_bBrtBeginColorScale && !m_arCFVO.empty() && !m_arBrtColor.empty() && m_bBrtEndColorScale;
    }

	const bool COLORSCALE::saveContent(BinProcessor& proc)
	{
		proc.mandatory<BeginColorScale>();

		for (auto &item : m_arCFVO)
		{
			proc.mandatory(*item);
		}

		for (auto &item : m_arBrtColor)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndColorScale>();

		return true;
	}

} // namespace XLSB

