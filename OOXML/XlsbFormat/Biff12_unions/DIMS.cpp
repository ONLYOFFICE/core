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

#include "DIMS.h"
#include "../Biff12_records/BeginDims.h"
#include "../Biff12_unions/DIM.h"
#include "../Biff12_records/EndDims.h"

using namespace XLS;

namespace XLSB
{

    DIMS::DIMS()
    {
    }

    DIMS::~DIMS()
    {
    }

    BaseObjectPtr DIMS::clone()
    {
        return BaseObjectPtr(new DIMS(*this));
    }

    //DIMS = BrtBeginDims 1*DIM BrtEndDims
    const bool DIMS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginDims>())
        {
            m_BrtBeginDims = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<DIM>(0, 0);
        while(count > 0)
        {
            m_arDIM.insert(m_arDIM.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndDims>())
        {
            m_bBrtEndDims = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndDims = false;

        return m_BrtBeginDims && m_bBrtEndDims;
    }

	const bool DIMS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginDims == nullptr)
			m_BrtBeginDims = XLS::BaseObjectPtr(new XLSB::BeginDims());

		if (m_BrtBeginDims != nullptr)
		{
			auto ptrBrtBeginDims = static_cast<XLSB::BeginDims*>(m_BrtBeginDims.get());

			if (ptrBrtBeginDims != nullptr)
				ptrBrtBeginDims->cDims = m_arDIM.size();

			proc.mandatory(*m_BrtBeginDims);
		}

		for (auto &item : m_arDIM)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndDims>();

		return true;
	}

} // namespace XLSB

