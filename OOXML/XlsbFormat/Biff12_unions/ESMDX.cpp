/*
 * (c) Copyright Ascensio System SIA 2010-2024
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

#include "ESMDX.h"
#include "MDX.h"

#include "../Biff12_records/BeginEsmdx.h"
#include "../Biff12_records/EndEsmdx.h"

using namespace XLS;

namespace XLSB
{

    ESMDX::ESMDX()
    {
    }

    ESMDX::~ESMDX()
    {
    }

    BaseObjectPtr ESMDX::clone()
    {
        return BaseObjectPtr(new ESMDX(*this));
    }

    const bool ESMDX::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginEsmdx>())
        {
			m_BrtBeginEsmdx = elements_.back();
            elements_.pop_back();
        }
		else
			return false;

        int count = proc.repeated<MDX>(0, 0);

        while(count > 0)
        {
            MDXs.insert(MDXs.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }
        if (proc.optional<EndEsmdx>())
        {
            m_bBrtEndEsmdx = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndEsmdx = false;

        return m_BrtBeginEsmdx && m_bBrtEndEsmdx;
    }

	const bool ESMDX::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginEsmdx == nullptr)
        {
            auto beginPtr(new BeginEsmdx);
            beginPtr->cMdx = MDXs.size();
            m_BrtBeginEsmdx = BaseObjectPtr{beginPtr};
        }
        proc.mandatory(*m_BrtBeginEsmdx);

		for(auto i:MDXs)
            proc.mandatory(*i);
        proc.mandatory<XLSB::EndEsmdx>();
		
		return true;
	}

} // namespace XLSB

