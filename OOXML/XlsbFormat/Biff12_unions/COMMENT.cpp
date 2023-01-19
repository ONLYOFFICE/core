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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

#include "COMMENT.h"
#include "../Biff12_unions/ACUID.h"
#include "../Biff12_records/BeginComment.h"
#include "../Biff12_records/CommentText.h"
#include "../Biff12_records/EndComment.h"

using namespace XLS;

namespace XLSB
{

    COMMENT::COMMENT()
    {
    }

    COMMENT::~COMMENT()
    {
    }

    BaseObjectPtr COMMENT::clone()
    {
        return BaseObjectPtr(new COMMENT(*this));
    }

    //COMMENT = [ACUID] BrtBeginComment [BrtCommentText] BrtEndComment
    const bool COMMENT::loadContent(BinProcessor& proc)
    {
        if (proc.optional<ACUID>())
        {
            //m_ACUID = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<BeginComment>())
        {
            m_BrtBeginComment = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<CommentText>())
        {
            m_BrtCommentText = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<EndComment>())
        {
            m_BrtEndComment = elements_.back();
            elements_.pop_back();
        }

        return m_BrtBeginComment && m_BrtEndComment;
    }

} // namespace XLSB

