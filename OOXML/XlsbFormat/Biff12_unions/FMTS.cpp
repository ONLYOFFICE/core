/*
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

#include "FMTS.h"
#include "../Biff12_records/BeginFmts.h"
#include "../Biff12_unions/ACFMT.h"
#include "../Biff12_records/Fmt.h"
#include "../Biff12_records/EndFmts.h"

using namespace XLS;

namespace XLSB
{

    FMTS::FMTS()
    {
    }

    FMTS::~FMTS()
    {
    }

    BaseObjectPtr FMTS::clone()
    {
        return BaseObjectPtr(new FMTS(*this));
    }

    //FMTS = BrtBeginFmts 1*(2ACFMT / BrtFmt) BrtEndFmts
    const bool FMTS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginFmts>())
        {
            m_BrtBeginFmts = elements_.back();
            elements_.pop_back();
        }

        while(true)
        {
            if (proc.optional<Fmt>())
            {
				m_arBrtFmt.push_back(elements_.back());
                elements_.pop_back();
            }

            else if (proc.optional<ACFMT>())
            {
				m_arBrtFmt.push_back(elements_.back());
                elements_.pop_back();
            }

            else break;
        }

        if (proc.optional<EndFmts>())
        {
			m_bBrtEndFmts = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndFmts = false;

        return m_BrtBeginFmts && !m_arBrtFmt.empty() && m_bBrtEndFmts;
    }

	const bool FMTS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginFmts == nullptr)
			m_BrtBeginFmts = XLS::BaseObjectPtr(new XLSB::BeginFmts());

		if (m_BrtBeginFmts != nullptr)
		{
			auto ptrBrtBeginFmts = static_cast<XLSB::BeginFmts*>(m_BrtBeginFmts.get());

			if (ptrBrtBeginFmts != nullptr)
				ptrBrtBeginFmts->cfmts = m_arBrtFmt.size();

			proc.mandatory(*m_BrtBeginFmts);
		}

		for (auto &item : m_arBrtFmt)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndFmts>();

		return true;
	}

} // namespace XLSB

