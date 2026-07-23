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

#include "MDX.h"
#include "MDXMBRPROP.h"
#include "MDXKPI.h"
#include "MDXSET.h"
#include "MDXTUPLE.h"

#include "../Biff12_records/BeginMdx.h"
#include "../Biff12_records/EndMdx.h"

using namespace XLS;

namespace XLSB
{

    MDX::MDX()
    {
    }

    MDX::~MDX()
    {
    }

    BaseObjectPtr MDX::clone()
    {
        return BaseObjectPtr(new MDX(*this));
    }

    //MDX = BrtBeginMdx COMMENTAUTHORS COMMENTLIST *FRT BrtEndMdx
    const bool MDX::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginMdx>())
        {
			m_BrtBeginMdx = elements_.back();
            elements_.pop_back();
        }
        else
            m_BrtBeginMdx = BaseObjectPtr();

        if(proc.optional<MDXSET>())
        {
            m_MDXSET = elements_.back();
        }
        else if(proc.optional<MDXMBRPROP>())
        {
            m_MDXMBRPROP = elements_.back();
        }
        else if(proc.optional<MDXKPI>())
        {
            m_MDXKPI = elements_.back();
        }
        else if(proc.optional<MDXTUPLE>())
        {
            m_MDXTUPLE = elements_.back();
        }
        if (proc.optional<EndMdx>())
        {
            m_bBrtEndMdx = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndMdx = false;

        return true;
    }

	const bool MDX::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginMdx != nullptr)
			proc.mandatory(*m_BrtBeginMdx);
        if(m_MDXTUPLE != nullptr)
            proc.mandatory(*m_MDXTUPLE);
        else if(m_MDXSET != nullptr)
            proc.mandatory(*m_MDXSET);
        else if(m_MDXMBRPROP != nullptr)
            proc.mandatory(*m_MDXMBRPROP);
        else if(m_MDXKPI != nullptr)
            proc.mandatory(*m_MDXKPI);
        proc.mandatory<XLSB::EndMdx>();
		
		return true;
	}

} // namespace XLSB

