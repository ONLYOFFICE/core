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

#include "LISTXMLCPR.h"
#include "../Biff12_records/BeginListXmlCPr.h"
#include "../Biff12_records/EndListXmlCPr.h"
#include "../Biff12_unions/FRT.h"

using namespace XLS;

namespace XLSB
{

    LISTXMLCPR::LISTXMLCPR()
    {
    }

    LISTXMLCPR::~LISTXMLCPR()
    {
    }

    BaseObjectPtr LISTXMLCPR::clone()
    {
        return BaseObjectPtr(new LISTXMLCPR(*this));
    }

    // LISTXMLCPR = BrtBeginListXmlCPr *FRT BrtEndListXmlCPr
    const bool LISTXMLCPR::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginListXmlCPr>())
        {
            m_BrtBeginListXmlCPr = elements_.back();
            elements_.pop_back();
        }

        int count = proc.repeated<FRT>(0, 0);

        while(count > 0)
        {
            //m_arFRT.insert(m_arFRT.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndListXmlCPr>())
        {
			m_bBrtEndListXmlCPr = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndListXmlCPr = false;

        return m_BrtBeginListXmlCPr && m_bBrtEndListXmlCPr;
    }

	const bool LISTXMLCPR::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginListXmlCPr != nullptr)		
			proc.mandatory(*m_BrtBeginListXmlCPr);				

		proc.mandatory<EndListXmlCPr>();

		return true;
	}

} // namespace XLSB

