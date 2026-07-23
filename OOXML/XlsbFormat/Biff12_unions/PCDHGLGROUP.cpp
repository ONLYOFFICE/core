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

#include "PCDHGLGROUP.h"
#include "../Biff12_records/BeginPCDHGLGroup.h"
#include "../Biff12_unions/PCDHGLGMEMBERS.h"
#include "../Biff12_records/EndPCDHGLGroup.h"

using namespace XLS;

namespace XLSB
{

    PCDHGLGROUP::PCDHGLGROUP()
    {
    }

    PCDHGLGROUP::~PCDHGLGROUP()
    {
    }

    BaseObjectPtr PCDHGLGROUP::clone()
    {
        return BaseObjectPtr(new PCDHGLGROUP(*this));
    }

    //PCDHGLGROUP = BrtBeginPCDHGLGroup PCDHGLGMEMBERS BrtEndPCDHGLGroup
    const bool PCDHGLGROUP::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDHGLGroup>())
        {
            m_BrtBeginPCDHGLGroup = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PCDHGLGMEMBERS>())
        {
            m_PCDHGLGMEMBERS = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndPCDHGLGroup>())
        {
            m_bBrtEndPCDHGLGroup = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCDHGLGroup = false;

        return m_BrtBeginPCDHGLGroup && m_bBrtEndPCDHGLGroup;
    }

	const bool PCDHGLGROUP::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPCDHGLGroup != nullptr)
			proc.mandatory(*m_BrtBeginPCDHGLGroup);

		if (m_PCDHGLGMEMBERS != nullptr)
			proc.mandatory(*m_PCDHGLGMEMBERS);

		proc.mandatory<EndPCDHGLGroup>();

		return true;
	}

} // namespace XLSB

