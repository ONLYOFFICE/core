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

#include "FMD.h"
#include "FRT.h"
#include "DYNAMICARRAYMETADATA.h"
#include "RICHDATAMETADATA.h"

#include "../Biff12_records/BeginFmd.h"
#include "../Biff12_records/EndFmd.h"


using namespace XLS;

namespace XLSB
{

    FMD::FMD()
    {
    }

    FMD::~FMD()
    {
    }

    BaseObjectPtr FMD::clone()
    {
        return BaseObjectPtr(new FMD(*this));
    }

    //FMD = BrtBeginFmd COMMENTAUTHORS COMMENTLIST *FRT BrtEndFmd
    const bool FMD::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginFmd>())
        {
			m_BrtBeginFmd = true;
            elements_.pop_back();
        }
		else
			m_BrtBeginFmd = false;

       


        if (proc.optional<DYNAMICARRAYMETADATA>())
        {
            m_DYNAMICARRAYMETADATA = elements_.back();
            elements_.pop_back();
        }
        else if (proc.optional<RICHDATAMETADATA>())
        {
            m_RICHDATAMETADATA = elements_.back();
            elements_.pop_back();
        }
        else if(proc.optional<FRT>())
        {
            elements_.pop_back();
        }
        if (proc.optional<EndFmd>())
        {
            m_bBrtEndFmd = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndFmd = false;

        return m_BrtBeginFmd && m_bBrtEndFmd;
    }

	const bool FMD::saveContent(XLS::BinProcessor & proc)
	{
        proc.mandatory<BeginFmd>();
		if (m_DYNAMICARRAYMETADATA != nullptr)
			proc.mandatory(*m_DYNAMICARRAYMETADATA);
        else if(m_RICHDATAMETADATA != nullptr)
            proc.mandatory(*m_RICHDATAMETADATA);
        proc.mandatory<EndFmd>();
		return true;
	}

} // namespace XLSB

