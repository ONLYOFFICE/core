/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "COMMENTAUTHORS.h"
#include "../Biff12_records/BeginCommentAuthors.h"
#include "../Biff12_records/CommentAuthor.h"
#include "../Biff12_records/EndCommentAuthors.h"

using namespace XLS;

namespace XLSB
{

    COMMENTAUTHORS::COMMENTAUTHORS()
    {
    }

    COMMENTAUTHORS::~COMMENTAUTHORS()
    {
    }

    BaseObjectPtr COMMENTAUTHORS::clone()
    {
        return BaseObjectPtr(new COMMENTAUTHORS(*this));
    }

    //COMMENTAUTHORS = BrtBeginCommentAuthors *BrtCommentAuthor BrtEndCommentAuthors
    const bool COMMENTAUTHORS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginCommentAuthors>())
        {
            m_bBrtBeginCommentAuthors = true;
            elements_.pop_back();
        }
		else
			m_bBrtBeginCommentAuthors = false;

        int count = proc.repeated<CommentAuthor>(0, 0);

        while(count > 0)
        {
            m_arBrtCommentAuthor.insert(m_arBrtCommentAuthor.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }
        if (proc.optional<EndCommentAuthors>())
        {
            m_bBrtEndCommentAuthors = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndCommentAuthors = false;

        return m_bBrtBeginCommentAuthors && m_bBrtEndCommentAuthors;
    }

	const bool COMMENTAUTHORS::saveContent(XLS::BinProcessor & proc)
	{
		proc.mandatory<BeginCommentAuthors>();

		for (auto &item : m_arBrtCommentAuthor)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndCommentAuthors>();

		return true;
	}

} // namespace XLSB

