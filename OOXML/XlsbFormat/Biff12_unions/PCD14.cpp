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

#include "PCD14.h"
#include "../Biff12_records/BeginPCD14.h"
#include "../Biff12_records/EndPCD14.h"

using namespace XLS;

namespace XLSB
{

    PCD14::PCD14()
    {
    }

    PCD14::~PCD14()
    {
    }

    BaseObjectPtr PCD14::clone()
    {
        return BaseObjectPtr(new PCD14(*this));
    }

    //PCD14 = BrtBeginPCD14 BrtEndPCD14
    const bool PCD14::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCD14>())
        {
            m_BrtBeginPCD14 = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndPCD14>())
        {
            m_bBrtEndPCD14 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCD14 = false;

        return m_BrtBeginPCD14 && m_bBrtEndPCD14;
    }

	const bool PCD14::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPCD14 != nullptr)
			proc.mandatory(*m_BrtBeginPCD14);

		proc.mandatory<EndPCD14>();

		return true;
	}

} // namespace XLSB

