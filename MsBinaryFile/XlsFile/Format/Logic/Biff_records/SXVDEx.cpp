/*
 * (c) Copyright Ascensio System SIA 2010-2023
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

#include "SXVDEx.h"

namespace XLS
{

SXVDEx::SXVDEx()
{
}

SXVDEx::~SXVDEx()
{
}

BaseObjectPtr SXVDEx::clone()
{
	return BaseObjectPtr(new SXVDEx(*this));
}


void SXVDEx::readFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();

	unsigned short	flags1;
	unsigned char	flags2;

	record >> flags1 >> flags2 >> citmAutoShow >> isxdiAutoSort >> isxdiAutoShow >> ifmt;
	ifmt = global_info->RegisterNumFormat(ifmt, L""); // return update
	
	fShowAllItems	= GETBIT(flags1, 0);
	fDragToRow		= GETBIT(flags1, 1);
	fDragToColumn	= GETBIT(flags1, 2);
	fDragToPage		= GETBIT(flags1, 3);
	fDragToHide		= GETBIT(flags1, 4);
	fNotDragToData	= GETBIT(flags1, 5);
	
	fServerBased	= GETBIT(flags1, 7);

	fAutoSort				= GETBIT(flags1, 9);
	fAscendSort				= GETBIT(flags1, 10);
	fAutoShow				= GETBIT(flags1, 11);
	fTopAutoShow			= GETBIT(flags1, 12);
	fCalculatedField		= GETBIT(flags1, 13);
	fPageBreaksBetweenItems	= GETBIT(flags1, 14);
	fHideNewItems			= GETBIT(flags1, 15);
	
	fOutline		= GETBIT(flags2, 5);
	fInsertBlankRow	= GETBIT(flags2, 6);
	fSubtotalAtTop	= GETBIT(flags2, 7);

	_UINT32	reserved1, reserved2;
	record >> cchSubName >> reserved1 >> reserved2;

	if (cchSubName > 0 && cchSubName < 0xffff)
	{
		stSubName.setSize(cchSubName);
		record >> stSubName;
	}
}

void SXVDEx::writeFields(CFRecord& record)
{
    unsigned short	flags1 = 0;
    unsigned char	flags2 = 0;
    SETBIT(flags1, 0, fShowAllItems)
    SETBIT(flags1, 1, fDragToRow)
    SETBIT(flags1, 2, fDragToColumn)
    SETBIT(flags1, 3, fDragToPage)
    SETBIT(flags1, 4, fDragToHide)
    SETBIT(flags1, 5, fNotDragToData)

    SETBIT(flags1, 7, fServerBased)

    SETBIT(flags1, 9, fAutoSort)
    SETBIT(flags1, 10, fAscendSort)
    SETBIT(flags1, 11, fAutoShow)
    SETBIT(flags1, 12, fTopAutoShow)
    SETBIT(flags1, 13, fCalculatedField)
    SETBIT(flags1, 14, fPageBreaksBetweenItems)
    SETBIT(flags1, 15, fHideNewItems)

    SETBIT(flags2, 5, fOutline)
    SETBIT(flags2, 6, fInsertBlankRow)
    SETBIT(flags2, 7, fSubtotalAtTop)
    record << flags1 << flags2 << citmAutoShow << isxdiAutoSort << isxdiAutoShow << ifmt;
    cchSubName = stSubName.getSize();
    if (cchSubName > 0 && cchSubName < 0xffff)
    {
        record << cchSubName;
        record.reserveNunBytes(8);
        record << stSubName;
    }
	else
	{
		cchSubName = 0xffff;
		record << cchSubName;
		record.reserveNunBytes(8);
	}
}

} // namespace XLS

