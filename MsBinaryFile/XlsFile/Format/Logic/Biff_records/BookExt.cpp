/*
 * (c) Copyright Ascensio System SIA 2010-2019
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

#include "BookExt.h"
#include "../Biff_structures/FrtHeader.h"

namespace XLS
{

BookExt::BookExt() : cb(22)
{
	mdFactoidDisplay = 0;
}


BookExt::~BookExt()
{
}


BaseObjectPtr BookExt::clone()
{
	return BaseObjectPtr(new BookExt(*this));
}

void BookExt::readFields(CFRecord& record)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        FrtHeader header(rt_BookExt);
        record >> header;

        record >> cb;
        _UINT32 flags;

        if (record.loadAnyData(flags))
        {
            fDontAutoRecover		= GETBIT(flags, 0);
            fHidePivotList			= GETBIT(flags, 1);
            fFilterPrivacy			= GETBIT(flags, 2);
            fEmbedFactoids			= GETBIT(flags, 3);
            mdFactoidDisplay		= GETBITS(flags, 4, 5);
            fSavedDuringRecovery	= GETBIT(flags, 6);
            fCreatedViaMinimalSave	= GETBIT(flags, 7);
            fOpenedViaDataRecovery	= GETBIT(flags, 8);
            fOpenedViaSafeLoad		= GETBIT(flags, 9);
        }
        else return;


        if(cb > 20)
        {
            record >> grbit1;
        }
        if(cb > 21)
        {
            record >> grbit2;
        }
    }
    else
    {
        unsigned char flags;

        if (record.loadAnyData(flags))
        {
            fDontAutoRecover		= GETBIT(flags, 0);
            fSavedDuringRecovery	= GETBIT(flags, 1);
            fCreatedViaMinimalSave	= GETBIT(flags, 2);
            fOpenedViaDataRecovery	= GETBIT(flags, 3);
            fOpenedViaSafeLoad		= GETBIT(flags, 4);
        }

    }



}

} // namespace XLS

