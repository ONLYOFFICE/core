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

#include "FRTSXVIEW15.h"
#include "../Biff12_unions/PIVOTVALUECELLS15.h"
#include "../Biff12_unions/PIVOTTABLEUISETTINGS.h"

using namespace XLS;

namespace XLSB
{

    FRTSXVIEW15::FRTSXVIEW15()
    {
    }

    FRTSXVIEW15::~FRTSXVIEW15()
    {
    }

    BaseObjectPtr FRTSXVIEW15::clone()
    {
        return BaseObjectPtr(new FRTSXVIEW15(*this));
    }

    //FRTSXVIEW15 = [PIVOTVALUECELLS15] [PIVOTTABLEUISETTINGS]
    const bool FRTSXVIEW15::loadContent(BinProcessor& proc)
    {
        if (proc.optional<PIVOTVALUECELLS15>())
        {
            m_PIVOTVALUECELLS15 = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PIVOTTABLEUISETTINGS>())
        {
            m_PIVOTTABLEUISETTINGS = elements_.back();
            elements_.pop_back();
        }

        return m_PIVOTVALUECELLS15 || m_PIVOTTABLEUISETTINGS;
    }

	const bool FRTSXVIEW15::saveContent(XLS::BinProcessor & proc)
	{
		if (m_PIVOTVALUECELLS15 != nullptr)
			proc.mandatory(*m_PIVOTVALUECELLS15);

		if (m_PIVOTTABLEUISETTINGS != nullptr)
			proc.mandatory(*m_PIVOTTABLEUISETTINGS);

		return true;
	}

} // namespace XLSB

