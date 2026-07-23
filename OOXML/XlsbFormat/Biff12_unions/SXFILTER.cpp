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

#include "SXFILTER.h"
#include "../Biff12_records/BeginSXFilter.h"
#include "../Biff12_unions/SXVIS.h"
#include "../Biff12_unions/AFILTER.h"
#include "../Biff12_unions/FRTSXFILTER.h"
#include "../Biff12_records/EndSXFilter.h"

using namespace XLS;

namespace XLSB
{

    SXFILTER::SXFILTER()
    {
    }

    SXFILTER::~SXFILTER()
    {
    }

    BaseObjectPtr SXFILTER::clone()
    {
        return BaseObjectPtr(new SXFILTER(*this));
    }

    //SXFILTER = BrtBeginSXFILTER AFILTER FRTSXFILTER BrtEndSXFilter
    const bool SXFILTER::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSXFilter>())
        {
            m_BrtBeginSXFilter = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<AFILTER>())
        {
            m_AFILTER = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<FRTSXFILTER>())
        {
            m_FRTSXFILTER = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<EndSXFilter>())
        {
            m_bBrtEndSXFilter = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSXFilter = false;

        return m_BrtBeginSXFilter && m_bBrtEndSXFilter;
    }

	const bool SXFILTER::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginSXFilter != nullptr)
			proc.mandatory(*m_BrtBeginSXFilter);

		if (m_AFILTER != nullptr)
			proc.mandatory(*m_AFILTER);

		if (m_FRTSXFILTER != nullptr)
			proc.mandatory(*m_FRTSXFILTER);

		proc.mandatory<EndSXFilter>();

		return true;
	}

} // namespace XLSB

