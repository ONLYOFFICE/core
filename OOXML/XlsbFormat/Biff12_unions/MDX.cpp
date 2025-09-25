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

