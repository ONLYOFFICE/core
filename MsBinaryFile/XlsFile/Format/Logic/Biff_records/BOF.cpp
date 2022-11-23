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

#include "BOF.h"

#include "../../../../../Common/MS-LCID.h"

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

void BOF::readFields(CFRecord& record)
{
	type_id_ = record.getTypeId();

	record >> vers >> dt;
	
	if (type_id_ == rt_BOF_BIFF8)
	{
		record >> rupBuild >> rupYear;  // biff 5 - 8
		
		if ( record.checkFitReadSafe(8)) // biff 8
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
			
			verXLHigh = GETBITS(flags, 14, 17);

			record >> verLowestBiff;
			unsigned char flags2;
			record >> flags2;
			verLastXLSaved = GETBITS(flags2, 0, 3);
			
			stream_ptr = record.getStreamPointer();
			record.skipNunBytes(2); // reserved
		}
		else
		{ //ts_2500_06_gruzi 05 06 вып.xls
			if (record.getGlobalWorkbookInfo()->CodePage == 0 && record.getGlobalWorkbookInfo()->lcid_user > 0)
			{
				record.getGlobalWorkbookInfo()->CodePage = msLCID2DefCodePage(record.getGlobalWorkbookInfo()->lcid_user);
			}
		}
	}
	else
	{
		if (type_id_ == rt_BOF_BIFF3 || type_id_ == rt_BOF_BIFF4)
		{
			short not_used;
			record >> not_used;
		}

		switch(type_id_)
		{
		case rt_BOF_BIFF8: verLowestBiff = 6; break;
		case rt_BOF_BIFF4: verLowestBiff = 4; vers = 0x0400; break;
		case rt_BOF_BIFF3: verLowestBiff = 3; vers = 0x0300; break;
		case rt_BOF_BIFF2: verLowestBiff = 2; vers = 0x0200; break;;
		default:
			break;
		}		 
	}
}

unsigned short BOF::getSubstreamType()
{
	return dt;
}


} // namespace XLS


