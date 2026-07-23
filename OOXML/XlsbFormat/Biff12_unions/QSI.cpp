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

#include "QSI.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_unions/QSIR.h"
#include "../Biff12_unions/FRTQSI.h"
#include "../Biff12_records/EndQSI.h"

using namespace XLS;

namespace XLSB
{

    QSI::QSI()
    {
    }

    QSI::~QSI()
    {
    }

    BaseObjectPtr QSI::clone()
    {
        return BaseObjectPtr(new QSI(*this));
    }

    // QSI = BrtBeginQSI [QSIR] FRTQSI BrtEndQSI
    const bool QSI::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginQSI>())
        {
            m_BrtBeginQSI = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<QSIR>())
        {
            m_QSIR = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<FRTQSI>())
        {
            m_FRTQSI = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndQSI>())
        {
			m_bBrtEndQSI = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndQSI = false;

        return m_BrtBeginQSI && m_bBrtEndQSI;
    }

	const bool QSI::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginQSI != nullptr)
			proc.mandatory(*m_BrtBeginQSI);

		if (m_QSIR != nullptr)
			proc.mandatory(*m_QSIR);

		if (m_FRTQSI != nullptr)
			proc.mandatory(*m_FRTQSI);

		proc.mandatory<EndQSI>();

		return true;
	}

} // namespace XLSB

