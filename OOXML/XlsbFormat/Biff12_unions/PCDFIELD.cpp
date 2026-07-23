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

#include "PCDFIELD.h"
#include "../Biff12_records/BeginPCDField.h"
#include "../Biff12_unions/PNAMES.h"
#include "../Biff12_unions/PCDFATBL.h"
#include "../Biff12_unions/PCDFGROUP.h"
#include "../Biff12_unions/FRTPCDFIELD.h"
#include "../Biff12_records/EndPCDField.h"

using namespace XLS;

namespace XLSB
{

    PCDFIELD::PCDFIELD()
    {
    }

    PCDFIELD::~PCDFIELD()
    {
    }

    BaseObjectPtr PCDFIELD::clone()
    {
        return BaseObjectPtr(new PCDFIELD(*this));
    }

    //PCDFIELD = BrtBeginPCDField [PNAMES] [PCDFATBL] [PCDFGROUP] FRTPCDFIELD BrtEndPCDField
    const bool PCDFIELD::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDField>())
        {
            m_BrtBeginPCDField = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PNAMES>())
        {
            m_PNAMES = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PCDFATBL>())
        {
            m_PCDFATBL = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PCDFGROUP>())
        {
            m_PCDFGROUP = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<FRTPCDFIELD>())
        {
            m_FRTPCDFIELD = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndPCDField>())
        {
			m_bBrtEndPCDField = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCDField = false;
        global_info = proc.getGlobalWorkbookInfo();

        return m_BrtBeginPCDField && m_bBrtEndPCDField;
    }

	const bool PCDFIELD::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPCDField != nullptr)
			proc.mandatory(*m_BrtBeginPCDField);

		if (m_PNAMES != nullptr)
			proc.mandatory(*m_PNAMES);

		if (m_PCDFATBL != nullptr)
			proc.mandatory(*m_PCDFATBL);

		if (m_PCDFGROUP != nullptr)
			proc.mandatory(*m_PCDFGROUP);

		if (m_FRTPCDFIELD != nullptr)
			proc.mandatory(*m_FRTPCDFIELD);

		proc.mandatory<EndPCDField>();

		return true;
	}

} // namespace XLSB

