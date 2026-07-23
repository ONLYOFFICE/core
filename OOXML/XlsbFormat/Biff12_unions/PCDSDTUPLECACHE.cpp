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

#include "PCDSDTUPLECACHE.h"
#include "../Biff12_records/BeginPCDSDTupleCache.h"
#include "../Biff12_unions/PCDSDTCENTRIES.h"
#include "../Biff12_unions/PCDSDTCSETS.h"
#include "../Biff12_unions/PCDSDTCQUERIES.h"
#include "../Biff12_unions/PCDSFCIENTRIES.h"
#include "../Biff12_unions/FRT.h"
#include "../Biff12_records/EndPCDSDTupleCache.h"

using namespace XLS;

namespace XLSB
{

    PCDSDTUPLECACHE::PCDSDTUPLECACHE()
    {
    }

    PCDSDTUPLECACHE::~PCDSDTUPLECACHE()
    {
    }

    BaseObjectPtr PCDSDTUPLECACHE::clone()
    {
        return BaseObjectPtr(new PCDSDTUPLECACHE(*this));
    }

    //PCDSDTUPLECACHE = BrtBeginPCDSDTupleCache [PCDSDTCENTRIES] [PCDSDTCSETS] [PCDSDTCQUERIES] [PCDSFCIENTRIES] *FRT BrtEndPCDSDTupleCache
    const bool PCDSDTUPLECACHE::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDSDTupleCache>())
        {
			m_bBrtBeginPCDSDTupleCache = true;
            elements_.pop_back();
        }
		else
			m_bBrtBeginPCDSDTupleCache = false;

        if (proc.optional<PCDSDTCENTRIES>())
        {
            m_PCDSDTCENTRIES = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PCDSDTCSETS>())
        {
            m_PCDSDTCSETS = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PCDSDTCQUERIES>())
        {
            m_PCDSDTCQUERIES = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PCDSFCIENTRIES>())
        {
            m_PCDSFCIENTRIES = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<FRT>(0, 0);
        while(count > 0)
        {
            //m_arFRT.insert(m_arFRT.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndPCDSDTupleCache>())
        {
            m_bBrtEndPCDSDTupleCache = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCDSDTupleCache = false;

        return m_bBrtBeginPCDSDTupleCache && m_bBrtEndPCDSDTupleCache;
    }

	const bool PCDSDTUPLECACHE::saveContent(XLS::BinProcessor & proc)
	{
		proc.mandatory<BeginPCDSDTupleCache>();

		if (m_PCDSDTCENTRIES != nullptr)
			proc.mandatory(*m_PCDSDTCENTRIES);

		if (m_PCDSDTCSETS != nullptr)
			proc.mandatory(*m_PCDSDTCSETS);

		if (m_PCDSDTCQUERIES != nullptr)
			proc.mandatory(*m_PCDSDTCQUERIES);

		if (m_PCDSFCIENTRIES != nullptr)
			proc.mandatory(*m_PCDSFCIENTRIES);

		proc.mandatory<EndPCDSDTupleCache>();

		return true;
	}

} // namespace XLSB

