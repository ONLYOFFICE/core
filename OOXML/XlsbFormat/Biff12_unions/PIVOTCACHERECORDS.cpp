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

#include "PIVOTCACHERECORDS.h"
#include "../Biff12_records/BeginPivotCacheRecords.h"
#include "../Biff12_unions/PIVOTCACHERECORD.h"
#include "../Biff12_unions/FRT.h"
#include "../Biff12_records/EndPivotCacheRecords.h"

using namespace XLS;

namespace XLSB
{

    PIVOTCACHERECORDS::PIVOTCACHERECORDS()
    {
    }

    PIVOTCACHERECORDS::~PIVOTCACHERECORDS()
    {
    }

    BaseObjectPtr PIVOTCACHERECORDS::clone()
    {
        return BaseObjectPtr(new PIVOTCACHERECORDS(*this));
    }

    // PIVOTCACHERECORDS = BrtBeginPivotCacheRecords *PIVOTCACHERECORD *FRT BrtEndPivotCacheRecords
    const bool PIVOTCACHERECORDS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPivotCacheRecords>())
        {
            m_BrtBeginPivotCacheRecords = elements_.back();
            elements_.pop_back();
        }

        int countPIVOTCACHERECORD = proc.repeated<PIVOTCACHERECORD>(0, 0);
		
		m_arPIVOTCACHERECORD.reserve(countPIVOTCACHERECORD);
		std::move(std::begin(elements_), std::end(elements_), std::back_inserter(m_arPIVOTCACHERECORD));

        int count = proc.repeated<FRT>(0, 0);
		elements_.clear();
        //while(count > 0)
        //{
        //    //m_arFRT.insert(m_arFRT.begin(), elements_.back());
        //    elements_.pop_back();
        //    count--;
        //}

        if (proc.optional<EndPivotCacheRecords>())
        {
            m_bBrtEndPivotCacheRecords = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPivotCacheRecords = false;

        return m_BrtBeginPivotCacheRecords && !m_arPIVOTCACHERECORD.empty() && m_bBrtEndPivotCacheRecords;
    }

	const bool PIVOTCACHERECORDS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPivotCacheRecords == nullptr)
			m_BrtBeginPivotCacheRecords = XLS::BaseObjectPtr(new XLSB::BeginPivotCacheRecords());

		if (m_BrtBeginPivotCacheRecords != nullptr)
		{
			auto ptrBrtBeginPivotCacheRecords = static_cast<XLSB::BeginPivotCacheRecords*>(m_BrtBeginPivotCacheRecords.get());

			if (ptrBrtBeginPivotCacheRecords != nullptr)
				ptrBrtBeginPivotCacheRecords->crecords = m_arPIVOTCACHERECORD.size();

			proc.mandatory(*m_BrtBeginPivotCacheRecords);
		}

		for (auto &item : m_arPIVOTCACHERECORD)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndPivotCacheRecords>();

		return true;
	}

} // namespace XLSB

