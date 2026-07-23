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

#include "SLICERCACHEOLAPIMPL.h"
#include "../Biff12_records/BeginSlicerCacheOlapImpl.h"
#include "../Biff12_unions/SLICERCACHELEVELSDATA.h"
#include "../Biff12_unions/SLICERCACHESELECTIONS.h"
#include "../Biff12_unions/FRT.h"
#include "../Biff12_records/EndSlicerCacheOlapImpl.h"

using namespace XLS;

namespace XLSB
{

    SLICERCACHEOLAPIMPL::SLICERCACHEOLAPIMPL()
    {
    }

    SLICERCACHEOLAPIMPL::~SLICERCACHEOLAPIMPL()
    {
    }

    BaseObjectPtr SLICERCACHEOLAPIMPL::clone()
    {
        return BaseObjectPtr(new SLICERCACHEOLAPIMPL(*this));
    }

    // SLICERCACHEOLAPIMPL = BrtBeginSlicerCacheOlapImpl SLICERCACHELEVELSDATA SLICERCACHESELECTIONS *FRT BrtEndSlicerCacheOlapImpl
    const bool SLICERCACHEOLAPIMPL::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSlicerCacheOlapImpl>())
        {
            m_BrtBeginSlicerCacheOlapImpl = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<SLICERCACHELEVELSDATA>())
        {
            m_SLICERCACHELEVELSDATA = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<SLICERCACHESELECTIONS>())
        {
            m_SLICERCACHESELECTIONS = elements_.back();
            elements_.pop_back();
        }

        int count = proc.repeated<FRT>(0, 0);

        while(count > 0)
        {
            //m_arFRT.insert(m_arFRT.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndSlicerCacheOlapImpl>())
        {
            m_bBrtEndSlicerCacheOlapImpl = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSlicerCacheOlapImpl = false;

        return m_BrtBeginSlicerCacheOlapImpl && m_SLICERCACHELEVELSDATA && m_SLICERCACHESELECTIONS && m_bBrtEndSlicerCacheOlapImpl;
    }

	const bool SLICERCACHEOLAPIMPL::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginSlicerCacheOlapImpl != nullptr)
			proc.mandatory(*m_BrtBeginSlicerCacheOlapImpl);

		if (m_SLICERCACHELEVELSDATA != nullptr)
			proc.mandatory(*m_SLICERCACHELEVELSDATA);

		if (m_SLICERCACHESELECTIONS != nullptr)
			proc.mandatory(*m_SLICERCACHESELECTIONS);

		proc.mandatory<EndSlicerCacheOlapImpl>();

		return true;
	}

} // namespace XLSB

