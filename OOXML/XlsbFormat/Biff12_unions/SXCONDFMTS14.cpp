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

#include "SXCONDFMTS14.h"
#include "../Biff12_records/BeginSXCondFmts14.h"
#include "../Biff12_unions/SXCONDFMT14.h"
#include "../Biff12_records/EndSXCondFmts14.h"

using namespace XLS;

namespace XLSB
{

    SXCONDFMTS14::SXCONDFMTS14()
    {
    }

    SXCONDFMTS14::~SXCONDFMTS14()
    {
    }

    BaseObjectPtr SXCONDFMTS14::clone()
    {
        return BaseObjectPtr(new SXCONDFMTS14(*this));
    }

    //SXCONDFMTS14 = BrtBeginSXCondFmts14 1*SXCONDFMT14 BrtEndSXCondFmts14
    const bool SXCONDFMTS14::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSXCondFmts14>())
        {
            m_BrtBeginSXCondFmts14 = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<SXCONDFMT14>(0, 0);
        while(count > 0)
        {
            m_arSXCONDFMT14.insert(m_arSXCONDFMT14.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndSXCondFmts14>())
        {
            m_bBrtEndSXCondFmts14 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSXCondFmts14 = false;

        return m_BrtBeginSXCondFmts14 && !m_arSXCONDFMT14.empty() && m_bBrtEndSXCondFmts14;
    }

	const bool SXCONDFMTS14::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginSXCondFmts14 == nullptr)
			m_BrtBeginSXCondFmts14 = XLS::BaseObjectPtr(new XLSB::BeginSXCondFmts14());

		if (m_BrtBeginSXCondFmts14 != nullptr)
		{
			auto ptrBrtBeginSXCondFmts14 = static_cast<XLSB::BeginSXCondFmts14*>(m_BrtBeginSXCondFmts14.get());

			if (ptrBrtBeginSXCondFmts14 != nullptr)
				ptrBrtBeginSXCondFmts14->csxcondfmts = m_arSXCONDFMT14.size();

			proc.mandatory(*m_BrtBeginSXCondFmts14);
		}

		for (auto &item : m_arSXCONDFMT14)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndSXCondFmts14>();

		return true;
	}

} // namespace XLSB

