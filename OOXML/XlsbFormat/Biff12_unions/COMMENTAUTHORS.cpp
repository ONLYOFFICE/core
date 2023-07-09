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

