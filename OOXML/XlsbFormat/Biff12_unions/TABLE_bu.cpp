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

#include "TABLE.h"
#include "../Biff12_unions/ACUID.h"
#include "../Biff12_records/BeginList.h"
#include "../Biff12_unions/AUTOFILTER.h"
#include "../Biff12_unions/SORTSTATE.h"
#include "../Biff12_unions/LISTCOLS.h"
#include "../Biff12_records/TableStyleClient.h"
#include "../Biff12_unions/FRTTABLE.h"
#include "../Biff12_records/EndList.h"

using namespace XLS;

namespace XLSB
{

    TABLE::TABLE()
    {
    }

    TABLE::~TABLE()
    {
    }

    BaseObjectPtr TABLE::clone()
    {
        return BaseObjectPtr(new TABLE(*this));
    }

    // TABLE = [ACUID] BrtBeginList [AUTOFILTER] [SORTSTATE] LISTCOLS BrtTableStyleClient FRTTABLE BrtEndList
    const bool TABLE::loadContent(BinProcessor& proc)
    {
        if (proc.optional<ACUID>())
        {
            //m_ACUID = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<BeginList>())
        {
            m_BrtBeginList = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<AUTOFILTER>())
        {
            m_AUTOFILTER = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<SORTSTATE>())
        {
            m_SORTSTATE = elements_.back();
            elements_.pop_back();
        }

        _UINT32 index = 0;
        if(m_BrtBeginList != nullptr)
            index = dynamic_cast<BeginList*>(m_BrtBeginList.get())->idList;

        LISTCOLS listcols;
        listcols.indexList = index;
        if (proc.optional(listcols))
        {
            m_LISTCOLS = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<TableStyleClient>())
        {
            m_BrtTableStyleClient = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<FRTTABLE>())
        {
            m_FRTTABLE = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndList>())
        {
            m_BrtEndList = elements_.back();
            elements_.pop_back();
        }

        return m_BrtBeginList && m_LISTCOLS && m_BrtTableStyleClient && m_BrtEndList;
    }

} // namespace XLSB

