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

#include "BeginPCDHierarchy.h"

using namespace XLS;

namespace XLSB
{

    BeginPCDHierarchy::BeginPCDHierarchy()
    {
    }

    BeginPCDHierarchy::~BeginPCDHierarchy()
    {
    }

    BaseObjectPtr BeginPCDHierarchy::clone()
    {
        return BaseObjectPtr(new BeginPCDHierarchy(*this));
    }

    void BeginPCDHierarchy::readFields(XLS::CFRecord& record)
    {
        _UINT16 flags1;
        BYTE    flags2;

        record >> flags1 >> cLevels >> isetParent >> iconSet >> flags2 >> wAttributeMemberValueType;

        fMeasure                        = GETBIT(flags1, 0);
        fSet                            = GETBIT(flags1, 1);
        fAttributeHierarchy             = GETBIT(flags1, 2);
        fMeasureHierarchy               = GETBIT(flags1, 3);
        fOnlyOneField                   = GETBIT(flags1, 4);
        fTimeHierarchy                  = GETBIT(flags1, 5);
        fKeyAttributeHierarchy          = GETBIT(flags1, 6);
        fAttributeMemberValueTypeKnown  = GETBIT(flags1, 7);
        fUnbalancedRealKnown            = GETBIT(flags1, 8);
        fUnbalancedReal                 = GETBIT(flags1, 9);
        fUnbalancedGroupKnown           = GETBIT(flags1, 10);
        fUnbalancedGroup                = GETBIT(flags1, 11);
        fHidden                         = GETBIT(flags1, 12);

        fLoadDimUnq                     = GETBIT(flags2, 0);
        fLoadDefaultUnq                 = GETBIT(flags2, 1);
        fLoadAllUnq                     = GETBIT(flags2, 2);
        fLoadAllDisp                    = GETBIT(flags2, 3);
        fLoadDispFld                    = GETBIT(flags2, 4);
        fLoadMeasGrp                    = GETBIT(flags2, 5);

        record >> stUnique >> stCaption;

        if(fLoadDimUnq)
            record >> stDimUnq;

        if(fLoadDefaultUnq)
            record >> stDefaultUnq;

        if(fLoadAllUnq)
            record >> stAllUnq;

        if(fLoadAllDisp)
            record >> stAllDisp;

        if(fLoadDispFld)
            record >> stDispFld;

        if(fLoadMeasGrp)
            record >> stMeasGrp;

    }

} // namespace XLSB

