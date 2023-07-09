﻿/*
 * (c) Copyright Ascensio System SIA 2010-2021
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "PNAME.h"
#include "../Biff12_records/BeginPName.h"
#include "../Biff12_unions/PNPAIRS.h"
#include "../Biff12_records/EndPName.h"

using namespace XLS;

namespace XLSB
{

    PNAME::PNAME()
    {
    }

    PNAME::~PNAME()
    {
    }

    BaseObjectPtr PNAME::clone()
    {
        return BaseObjectPtr(new PNAME(*this));
    }

    //PNAME = BrtBeginPName [PNPAIRS] BrtEndPName
    const bool PNAME::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPName>())
        {
            m_BrtBeginPName = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PNPAIRS>())
        {
            m_PNPAIRS = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndPName>())
        {
			m_bBrtEndPName = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPName = false;

        return m_BrtBeginPName && m_bBrtEndPName;
    }

	const bool PNAME::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPName != nullptr)
			proc.mandatory(*m_BrtBeginPName);

		if (m_PNPAIRS != nullptr)
			proc.mandatory(*m_PNPAIRS);

		proc.mandatory<EndPName>();

		return true;
	}

} // namespace XLSB

