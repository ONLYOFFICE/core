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

#include "SXVIEW14.h"
#include "../Biff12_records/BeginSXView14.h"
#include "../Biff12_unions/SXCHANGES.h"
#include "../Biff12_unions/SXEDITS.h"
#include "../Biff12_unions/SXCONDFMTS14.h"
#include "../Biff12_records/EndSXView14.h"

using namespace XLS;

namespace XLSB
{

    SXVIEW14::SXVIEW14()
    {
    }

    SXVIEW14::~SXVIEW14()
    {
    }

    BaseObjectPtr SXVIEW14::clone()
    {
        return BaseObjectPtr(new SXVIEW14(*this));
    }

    //SXVIEW14 = BrtBeginSXView14 [SXCHANGES] [SXEDITS] [SXCONDFMTS14] BrtEndSXView14
    const bool SXVIEW14::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSXView14>())
        {
            m_BrtBeginSXView14 = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<SXCHANGES>())
        {
            m_SXCHANGES = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<SXEDITS>())
        {
            m_SXEDITS = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<SXCONDFMTS14>())
        {
            m_SXCONDFMTS14 = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<EndSXView14>())
        {
            m_BrtEndSXView14 = elements_.back();
            elements_.pop_back();
        }

        return m_BrtBeginSXView14 && m_BrtEndSXView14;
    }

} // namespace XLSB

