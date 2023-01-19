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

#include "BeginSXTH.h"

using namespace XLS;

namespace XLSB
{

    BeginSXTH::BeginSXTH()
    {
    }

    BeginSXTH::~BeginSXTH()
    {
    }

    BaseObjectPtr BeginSXTH::clone()
    {
        return BaseObjectPtr(new BeginSXTH(*this));
    }

    void BeginSXTH::readFields(XLS::CFRecord& record)
    {
        _UINT32 flags1;
        _UINT16 flags2;
        record >> flags1 >> flags2;

        fOutlineMode                = GETBIT(flags1, 0);
        fEnableMultiplePageItems    = GETBIT(flags1, 1);
        fSubtotalAtTop              = GETBIT(flags1, 2);
        fDontShowFList              = GETBIT(flags1, 3);
        fDragToRow                  = GETBIT(flags1, 4);
        fDragToColumn               = GETBIT(flags1, 5);
        fDragToPage                 = GETBIT(flags1, 6);
        fDragToHide                 = GETBIT(flags1, 7);
        fDragToData                 = GETBIT(flags1, 8);
        fFilterInclusive            = GETBIT(flags1, 9);

        fLoadCap                    = GETBIT(flags2, 0);

        if(fLoadCap)
            record >> irstCaption;
    }

} // namespace XLSB

