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

#include "EXTERNALLINK.h"
#include "../Biff12_records/BeginSupBook.h"
#include "../Biff12_unions/EXTERNALBOOK.h"
#include "../Biff12_unions/DDEOLELINK.h"
#include "../Biff12_unions/FRT.h"
#include "../Biff12_records/EndSupBook.h"

using namespace XLS;

namespace XLSB
{

    EXTERNALLINK::EXTERNALLINK()
    {
    }

    EXTERNALLINK::~EXTERNALLINK()
    {
    }

    BaseObjectPtr EXTERNALLINK::clone()
    {
        return BaseObjectPtr(new EXTERNALLINK(*this));
    }

    //EXTERNALLINK = BrtBeginSupBook (EXTERNALBOOK / DDEOLELINK) *FRT BrtEndSupBook
    const bool EXTERNALLINK::loadContent(BinProcessor& proc)
    {
        ExternalReferenceType sbt;
        if (proc.optional<BeginSupBook>())
        {
            m_BrtBeginSupBook = elements_.back();
            sbt = dynamic_cast<BeginSupBook*>(m_BrtBeginSupBook.get())->sbt;
            elements_.pop_back();
        }

        EXTERNALBOOK externalbook(sbt);
        if (proc.optional(externalbook))
        {
            m_EXTERNALBOOK = elements_.back();
            elements_.pop_back();
        }

        DDEOLELINK ddeolelink(sbt);
        if (proc.optional(ddeolelink))
        {
            m_DDEOLELINK = elements_.back();
            elements_.pop_back();
        }

        int countFRT = proc.repeated<FRT>(0, 0);

        while(countFRT > 0)
        {
            //m_arFRT.insert(m_arFRT.begin(), elements_.back());
            elements_.pop_back();
            countFRT--;
        }

        if (proc.optional<EndSupBook>())
        {
            m_bBrtEndSupBook = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSupBook = false;

        return m_BrtBeginSupBook && m_bBrtEndSupBook;
    }

	const bool EXTERNALLINK::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginSupBook != nullptr)
			proc.mandatory(*m_BrtBeginSupBook);

		if (m_EXTERNALBOOK != nullptr)
			proc.mandatory(*m_EXTERNALBOOK);

		if (m_DDEOLELINK != nullptr)
			proc.mandatory(*m_DDEOLELINK);

		proc.mandatory<EndSupBook>();

		return true;
	}

} // namespace XLSB

