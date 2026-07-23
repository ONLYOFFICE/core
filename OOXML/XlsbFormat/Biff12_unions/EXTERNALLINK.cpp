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

#include "EXTERNALLINK.h"
#include "../Biff12_records/BeginSupBook.h"
#include "../Biff12_unions/EXTERNALBOOK.h"
#include "../Biff12_unions/DDEOLELINK.h"
#include "../Biff12_unions/FRT.h"
#include "../Biff12_records/EndSupBook.h"

using namespace XLS;

namespace XLSB
{

    EXTERNALLINK::EXTERNALLINK()
    {
    }

    EXTERNALLINK::~EXTERNALLINK()
    {
    }

    BaseObjectPtr EXTERNALLINK::clone()
    {
        return BaseObjectPtr(new EXTERNALLINK(*this));
    }

    //EXTERNALLINK = BrtBeginSupBook (EXTERNALBOOK / DDEOLELINK) *FRT BrtEndSupBook
    const bool EXTERNALLINK::loadContent(BinProcessor& proc)
    {
        ExternalReferenceType sbt;
        if (proc.optional<BeginSupBook>())
        {
            m_BrtBeginSupBook = elements_.back();
            sbt = dynamic_cast<BeginSupBook*>(m_BrtBeginSupBook.get())->sbt;
            elements_.pop_back();
        }

        EXTERNALBOOK externalbook(sbt);
        if (proc.optional(externalbook))
        {
            m_EXTERNALBOOK = elements_.back();
            elements_.pop_back();
        }

        DDEOLELINK ddeolelink(sbt);
        if (proc.optional(ddeolelink))
        {
            m_DDEOLELINK = elements_.back();
            elements_.pop_back();
        }

        int countFRT = proc.repeated<FRT>(0, 0);

        while(countFRT > 0)
        {
            //m_arFRT.insert(m_arFRT.begin(), elements_.back());
            elements_.pop_back();
            countFRT--;
        }

        if (proc.optional<EndSupBook>())
        {
            m_bBrtEndSupBook = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSupBook = false;

        return m_BrtBeginSupBook && m_bBrtEndSupBook;
    }

	const bool EXTERNALLINK::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginSupBook != nullptr)
			proc.mandatory(*m_BrtBeginSupBook);

		if (m_EXTERNALBOOK != nullptr)
			proc.mandatory(*m_EXTERNALBOOK);

		if (m_DDEOLELINK != nullptr)
			proc.mandatory(*m_DDEOLELINK);

		proc.mandatory<EndSupBook>();

		return true;
	}

} // namespace XLSB

