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

#include "Feat11WSSListInfo.h"
#include "BiffString.h"
#include "Xnum.h"

namespace XLS
{

Feat11WSSListInfo::Feat11WSSListInfo()
{
}

BiffStructurePtr Feat11WSSListInfo::clone()
{
	return BiffStructurePtr(new Feat11WSSListInfo(*this));
}

void Feat11WSSListInfo::load(CFRecord& record)
{
	_UINT32	flags1, reserved;
	_UINT16 unused2;
	unsigned char flags2;
	
	record >> LCID >> cDec >> flags1 >> flags2 >> bDefaultType >> unused2;

	switch(lfdt)
	{
	case 0x00000002://Number
	case 0x00000006://Currency
		rgbDV = BiffStructurePtr(new Xnum); break;
	case 0x00000003://Boolean
		rgbDV = BiffStructurePtr(new BIFF_DWORD); break;
	case 0x00000004:
		rgbDV = BiffStructurePtr(new DateAsNum); break;
	case 0x00000005://Invalid
	case 0x00000007://Invalid
	case 0x00000009://Invalid
	case 0x0000000a://Invalid
		break;
	case 0x00000001://Short Text
	case 0x00000008://Choice
	case 0x0000000b://Multi-choice
		rgbDV = BiffStructurePtr(new XLUnicodeString); break;
	}
	if (rgbDV)
		rgbDV->load(record);

	record >> strFormula;
	record >> reserved;


	fPercent		= GETBIT(flags1, 0);
	fDecSet			= GETBIT(flags1, 1);
	fDateOnly		= GETBIT(flags1, 2);
	fReadingOrder	= GETBITS(flags1, 3, 4);
	fRichText		= GETBIT(flags1, 5);
	fUnkRTFormatting= GETBIT(flags1, 6);
	fAlertUnkRTFormatting = GETBIT(flags1, 7);

	fReadOnly		= GETBIT(flags2, 0);
	fRequired		= GETBIT(flags2, 1);
	fMinSet			= GETBIT(flags2, 2);
	fMaxSet			= GETBIT(flags2, 3);
	fDefaultSet		= GETBIT(flags2, 4);
	fDefaultDateToday = GETBIT(flags2, 5);
	fLoadFormula	= GETBIT(flags2, 6);
	fAllowFillIn	= GETBIT(flags2, 6);
}


} // namespace XLS

