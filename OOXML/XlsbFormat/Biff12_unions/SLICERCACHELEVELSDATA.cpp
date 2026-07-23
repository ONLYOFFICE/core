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

#include "SLICERCACHELEVELSDATA.h"
#include "../Biff12_records/BeginSlicerCacheLevelsData.h"
#include "../Biff12_unions/SLICERCACHELEVELDATA.h"
#include "../Biff12_records/EndSlicerCacheLevelsData.h"

using namespace XLS;

namespace XLSB
{

    SLICERCACHELEVELSDATA::SLICERCACHELEVELSDATA()
    {
    }

    SLICERCACHELEVELSDATA::~SLICERCACHELEVELSDATA()
    {
    }

    BaseObjectPtr SLICERCACHELEVELSDATA::clone()
    {
        return BaseObjectPtr(new SLICERCACHELEVELSDATA(*this));
    }

    //SLICERCACHELEVELSDATA = BrtBeginSlicerCacheLevelsData 1*2147483647SLICERCACHELEVELDATA BrtEndSlicerCacheLevelsData
    const bool SLICERCACHELEVELSDATA::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSlicerCacheLevelsData>())
        {
            m_BrtBeginSlicerCacheLevelsData = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<SLICERCACHELEVELDATA>(0, 2147483647);
        while(count > 0)
        {
            m_arSLICERCACHELEVELDATA.insert(m_arSLICERCACHELEVELDATA.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndSlicerCacheLevelsData>())
        {
            m_bBrtEndSlicerCacheLevelsData = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSlicerCacheLevelsData = false;

        return m_BrtBeginSlicerCacheLevelsData && m_bBrtEndSlicerCacheLevelsData;
    }

	const bool SLICERCACHELEVELSDATA::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginSlicerCacheLevelsData == nullptr)
			m_BrtBeginSlicerCacheLevelsData = XLS::BaseObjectPtr(new XLSB::BeginSlicerCacheLevelsData());

		if (m_BrtBeginSlicerCacheLevelsData != nullptr)
		{
			auto ptrBrtBeginSlicerCacheLevelsData = static_cast<XLSB::BeginSlicerCacheLevelsData*>(m_BrtBeginSlicerCacheLevelsData.get());

			if (ptrBrtBeginSlicerCacheLevelsData != nullptr)
				ptrBrtBeginSlicerCacheLevelsData->clevels = m_arSLICERCACHELEVELDATA.size();

			proc.mandatory(*m_BrtBeginSlicerCacheLevelsData);
		}

		for (auto &item : m_arSLICERCACHELEVELDATA)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndSlicerCacheLevelsData>();

		return true;
	}

} // namespace XLSB

