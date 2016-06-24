/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "BOF.h"
#include <Binary/CFStream.h>


namespace XLS
{

BOF::BOF()
{
	fWin		= true;
	fRisc		= false;
	fBeta		= false;
	fWinAny		= true;
	fMacAny		= false;
	fBetaAny	= false;
	fRiscAny	= false;
	fOOM		= false;
	fGlJmp		= false;
	fFontLimit	= false;
	
	verXLHigh = 0;

	verLowestBiff	= 6;
	verLastXLSaved	= 0;
}


BOF::~BOF()
{
}


BaseObjectPtr BOF::clone()
{
	return BaseObjectPtr(new BOF(*this));
}


void BOF::writeFields(CFRecord& record)
{
	if(BOF::st_Worksheet == dt)
	{
		record.registerDelayedFilePointerSource(rt_BoundSheet8);
	}
	record << vers << dt << rupBuild << rupYear;
	
	_UINT32 flags = 0;
	SETBIT(flags, 0, fWin);
	SETBIT(flags, 1, fRisc);
	SETBIT(flags, 2, fBeta);
	SETBIT(flags, 3, fWinAny);
	SETBIT(flags, 4, fMacAny);
	SETBIT(flags, 5, fBetaAny);
	SETBIT(flags, 8, fRiscAny);
	SETBIT(flags, 9, fOOM);
	SETBIT(flags, 10, fGlJmp);
	SETBIT(flags, 13, fFontLimit);
	SETBITS(flags, 14,17, verXLHigh);
	record << flags;
	record << verLowestBiff;
	
	unsigned char flags2 = 0;
	SETBITS(flags2, 0, 3, verLastXLSaved);
	record << flags2;
	record.reserveNunBytes(2); // reserved
}


void BOF::readFields(CFRecord& record)
{
	record >> vers >> dt >> rupBuild >> rupYear;
	
	if ( record.checkFitReadSafe(8))
	{
		_UINT32 flags;
		record >> flags;
		
		fWin		= GETBIT(flags, 0);
		fRisc		= GETBIT(flags, 1);
		fBeta		= GETBIT(flags, 2);
		fWinAny		= GETBIT(flags, 3);
		fMacAny		= GETBIT(flags, 4);
		fBetaAny	= GETBIT(flags, 5);
		fRiscAny	= GETBIT(flags, 8);
		fOOM		= GETBIT(flags, 9);
		fGlJmp		= GETBIT(flags, 10);
		fFontLimit	= GETBIT(flags, 13);
		
		verXLHigh = static_cast<unsigned char>(GETBITS(flags, 14, 17));

		record >> verLowestBiff;
		unsigned char flags2;
		record >> flags2;
		verLastXLSaved = GETBITS(flags2, 0, 3);
		
		stream_ptr = record.getStreamPointer();
		record.skipNunBytes(2); // reserved
	}
}

unsigned short BOF::getSubstreamType()
{
	return dt;
}


} // namespace XLS


