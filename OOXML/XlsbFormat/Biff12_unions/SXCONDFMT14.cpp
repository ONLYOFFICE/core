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

#include "SXCONDFMT14.h"
#include "../Biff12_records/BeginSXCondFmt14.h"
#include "../Biff12_unions/PIVOTRULES14.h"
#include "../Biff12_records/EndSXCondFmt14.h"

using namespace XLS;

namespace XLSB
{

    SXCONDFMT14::SXCONDFMT14()
    {
    }

    SXCONDFMT14::~SXCONDFMT14()
    {
    }

    BaseObjectPtr SXCONDFMT14::clone()
    {
        return BaseObjectPtr(new SXCONDFMT14(*this));
    }

    //SXCONDFMT14 = BrtBeginSXCondFmt14 PIVOTRULES14 BrtEndSXCondFmt14
    const bool SXCONDFMT14::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSXCondFmt14>())
        {
            m_BrtBeginSXCondFmt14 = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PIVOTRULES14>())
        {
            m_PIVOTRULES14 = elements_.back();
            elements_.pop_back();
        }       

        if (proc.optional<EndSXCondFmt14>())
        {
            m_bBrtEndSXCondFmt14 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSXCondFmt14 = false;

        return m_BrtBeginSXCondFmt14 && m_bBrtEndSXCondFmt14;
    }

	const bool SXCONDFMT14::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginSXCondFmt14 != nullptr)
			proc.mandatory(*m_BrtBeginSXCondFmt14);

		if (m_PIVOTRULES14 != nullptr)
			proc.mandatory(*m_PIVOTRULES14);

		proc.mandatory<EndSXCondFmt14>();

		return true;
	}

} // namespace XLSB

