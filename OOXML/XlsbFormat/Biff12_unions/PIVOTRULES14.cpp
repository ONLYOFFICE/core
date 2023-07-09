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

#include "PIVOTRULES14.h"
#include "../Biff12_records/BeginSXRules14.h"
#include "../Biff12_unions/PIVOTRULE14.h"
#include "../Biff12_records/EndSXRules14.h"

using namespace XLS;

namespace XLSB
{

    PIVOTRULES14::PIVOTRULES14()
    {
    }

    PIVOTRULES14::~PIVOTRULES14()
    {
    }

    BaseObjectPtr PIVOTRULES14::clone()
    {
        return BaseObjectPtr(new PIVOTRULES14(*this));
    }

    //PIVOTRULES14 = BrtBeginSXRules14 PIVOTRULE14 BrtEndSXRules14  // возможно ошибка и должно быть *PIVOTRULE14
    const bool PIVOTRULES14::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSXRules14>())
        {
            m_BrtBeginSXRules14 = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PIVOTRULE14>())
        {
            m_PIVOTRULE14 = elements_.back();
            elements_.pop_back();
        }       

        if (proc.optional<EndSXRules14>())
        {
            m_bBrtEndSXRules14 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSXRules14 = false;

        return m_BrtBeginSXRules14 && m_bBrtEndSXRules14;
    }

	const bool PIVOTRULES14::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginSXRules14 != nullptr)
			proc.mandatory(*m_BrtBeginSXRules14);

		if (m_PIVOTRULE14 != nullptr)
			proc.mandatory(*m_PIVOTRULE14);

		proc.mandatory<EndSXRules14>();

		return true;
	}

} // namespace XLSB

