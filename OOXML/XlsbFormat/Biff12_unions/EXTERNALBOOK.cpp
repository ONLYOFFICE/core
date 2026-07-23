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

#include "EXTERNALBOOK.h"
#include "../Biff12_records/SupTabs.h"
#include "../Biff12_unions/EXTERNNAME.h"
#include "../Biff12_unions/EXTERNTABLE.h"

using namespace XLS;

namespace XLSB
{

    EXTERNALBOOK::EXTERNALBOOK(ExternalReferenceType type) : sbt(type)
    {
    }

    EXTERNALBOOK::~EXTERNALBOOK()
    {
    }

    BaseObjectPtr EXTERNALBOOK::clone()
    {
        return BaseObjectPtr(new EXTERNALBOOK(*this));
    }

    //EXTERNALBOOK = BrtSupTabs *EXTERNNAME *EXTERNTABLE
    const bool EXTERNALBOOK::loadContent(BinProcessor& proc)
    {
        if (proc.optional<SupTabs>())
        {
            m_BrtSupTabs = elements_.back();
            elements_.pop_back();
        }

        EXTERNNAME externname(sbt);
        int countEXTERNNAME = proc.repeated(externname, 0, 0);

        while(countEXTERNNAME > 0)
        {
            m_arEXTERNNAME.insert(m_arEXTERNNAME.begin(), elements_.back());
            elements_.pop_back();
            countEXTERNNAME--;
        }

        int countEXTERNTABLE = proc.repeated<EXTERNTABLE>(0, 0);

        while(countEXTERNTABLE > 0)
        {
            m_arEXTERNTABLE.insert(m_arEXTERNTABLE.begin(), elements_.back());
            elements_.pop_back();
            countEXTERNTABLE--;
        }

        return m_BrtSupTabs != nullptr;
    }

	const bool EXTERNALBOOK::saveContent(BinProcessor& proc)
	{
		if (m_BrtSupTabs != nullptr)
			proc.mandatory(*m_BrtSupTabs);

		for (auto &item : m_arEXTERNNAME)
		{
			proc.mandatory(*item);
		}

		for (auto &item : m_arEXTERNTABLE)
		{
			proc.mandatory(*item);
		}

		return true;
	}

} // namespace XLSB

