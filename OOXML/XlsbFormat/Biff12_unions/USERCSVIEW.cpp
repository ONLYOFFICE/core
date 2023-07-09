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

#include "USERCSVIEW.h"
#include "../Biff12_records/BeginUserCsView.h"
#include "../Biff12_records/Margins.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_unions/HEADERFOOTER.h"
#include "../Biff12_records/EndUserCsView.h"

using namespace XLS;

namespace XLSB
{

    USERCSVIEW::USERCSVIEW()
    {
    }

    USERCSVIEW::~USERCSVIEW()
    {
    }

    BaseObjectPtr USERCSVIEW::clone()
    {
        return BaseObjectPtr(new USERCSVIEW(*this));
    }

    // USERCSVIEW = BrtBeginUserCsView [BrtMargins] [BrtCsPageSetup] [HEADERFOOTER] BrtEndUserCsView
    const bool USERCSVIEW::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginUserCsView>())
        {
            m_BrtBeginUserCsView = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<Margins>())
        {
            m_BrtMargins = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<CsPageSetup>())
        {
            m_BrtCsPageSetup = elements_.back();
            elements_.pop_back();
        }       

        if (proc.optional<HEADERFOOTER>())
        {
            m_HEADERFOOTER = elements_.back();
            elements_.pop_back();
        }

		if (proc.optional<EndUserCsView>())
		{
			m_bBrtEndUserCsView = true;
			elements_.pop_back();
		}
		else
			m_bBrtEndUserCsView = false;

        return m_BrtBeginUserCsView && m_bBrtEndUserCsView;
    }

	const bool USERCSVIEW::saveContent(XLS::BinProcessor & proc)
	{	
		if (m_BrtBeginUserCsView != nullptr)
			proc.mandatory(*m_BrtBeginUserCsView);

		if (m_BrtMargins != nullptr)
			proc.mandatory(*m_BrtMargins);

		if (m_BrtCsPageSetup != nullptr)
			proc.mandatory(*m_BrtCsPageSetup);

		if (m_HEADERFOOTER != nullptr)
			proc.mandatory(*m_HEADERFOOTER);

		proc.mandatory<EndUserCsView>();

		return true;
	}

} // namespace XLSB

