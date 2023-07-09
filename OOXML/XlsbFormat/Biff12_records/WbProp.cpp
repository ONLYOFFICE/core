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

#include "WbProp.h"

using namespace XLS;

namespace XLSB
{

    WbProp::WbProp()
    {
    }

    WbProp::~WbProp()
    {
    }

    BaseObjectPtr WbProp::clone()
    {
        return BaseObjectPtr(new WbProp(*this));
    }

    void WbProp::readFields(XLS::CFRecord& record)
    {
        _UINT32 flags;
        record >> flags;

        f1904                    = GETBIT(flags, 0);
        fHideBorderUnselLists	 = GETBIT(flags, 2);
        fFilterPrivacy           = GETBIT(flags, 3);
        fBuggedUserAboutSolution = GETBIT(flags, 4);

        fShowInkAnnotation       = GETBIT(flags, 5);
        fBackup                  = GETBIT(flags, 6);
        fNoSaveSup               = GETBIT(flags, 7);
        grbitUpdateLinks         = GETBITS(flags, 8, 9);
        fHidePivotTableFList     = GETBIT(flags, 10);
        fPublishedBookItems      = GETBIT(flags, 11);
        fCheckCompat             = GETBIT(flags, 12);
        mdDspObj                 = GETBITS(flags, 13, 14);
        fShowPivotChartFilter    = GETBIT(flags, 15);
        fAutoCompressPictures    = GETBIT(flags, 16);
        fRefreshAll              = GETBIT(flags, 17);

        record >> dwThemeVersion;
        record >> strName;
    }

	void WbProp::writeFields(XLS::CFRecord& record)
	{
		_UINT32 flags = 0;

		SETBIT(flags, 0, f1904)
		SETBIT(flags, 2, fHideBorderUnselLists)
		SETBIT(flags, 3, fFilterPrivacy)
		SETBIT(flags, 4, fBuggedUserAboutSolution)
		SETBIT(flags, 5, fShowInkAnnotation)
		SETBIT(flags, 6, fBackup)
		SETBIT(flags, 7, fNoSaveSup)
		SETBITS(flags, 8, 9, grbitUpdateLinks)
		SETBIT(flags, 10, fHidePivotTableFList)
		SETBIT(flags, 11, fPublishedBookItems)
		SETBIT(flags, 12, fCheckCompat)
		SETBITS(flags, 13, 14, mdDspObj);
		SETBIT(flags, 15, fShowPivotChartFilter)
		SETBIT(flags, 16, fAutoCompressPictures)
		SETBIT(flags, 17, fRefreshAll)

		record << flags;
		record << dwThemeVersion;
		record << strName;
	}

} // namespace XLSB

