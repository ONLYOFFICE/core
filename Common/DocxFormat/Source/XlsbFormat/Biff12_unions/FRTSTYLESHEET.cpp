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

#include "FRTSTYLESHEET.h"
#include "../Biff12_unions/STYLESHEET14.h"
#include "../Biff12_unions/DXF14S.h"
#include "../Biff12_unions/STYLESHEET15.h"
#include "../Biff12_unions/DXFS15.h"

using namespace XLS;

namespace XLSB
{

    FRTSTYLESHEET::FRTSTYLESHEET()
    {
    }

    FRTSTYLESHEET::~FRTSTYLESHEET()
    {
    }

    BaseObjectPtr FRTSTYLESHEET::clone()
    {
        return BaseObjectPtr(new FRTSTYLESHEET(*this));
    }

    // FRTSTYLESHEET = [STYLESHEET14] [DXF14S] [STYLESHEET15] [DXFS15] *FRT
    const bool FRTSTYLESHEET::loadContent(BinProcessor& proc)
    {       
        if (proc.optional<STYLESHEET14>())
        {
            m_STYLESHEET14 = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<DXF14S>())
        {
            m_DXF14S = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<STYLESHEET15>())
        {
            m_STYLESHEET15 = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<DXFS15>())
        {
            m_DXFS15 = elements_.back();
            elements_.pop_back();
        }

        return m_STYLESHEET14 != nullptr || m_DXF14S != nullptr || m_STYLESHEET15 != nullptr || m_DXFS15 != nullptr;
    }

} // namespace XLSB

