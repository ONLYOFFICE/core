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

#include "TxtQry.h"

namespace XLS
{

TxtQry::TxtQry()
{
	iCpidNew	= 0;
	iTextDelm	= 0;
}


TxtQry::~TxtQry()
{
}


BaseObjectPtr TxtQry::clone()
{
	return BaseObjectPtr(new TxtQry(*this));
}


void TxtQry::readFields(CFRecord& record)
{
	unsigned short reserved, flags1, unused1;
	unsigned char unused2, flags2;

	record >> rt >> reserved >> flags1 >> unused1 >> rowStartAt >> flags2 >> chCustom >> unused2 >> itwf >> chDecimal >> chThousSep;

	fFile			=  GETBIT(flags1, 0);
	fDelimited		=  GETBIT(flags1, 1);
	iCpid			=  GETBITS(flags1, 2, 3);
	fPromptForFile	=  GETBIT(flags1, 4);
	iCpidNew		=  GETBITS(flags1, 5, 14);
	fUseNewiCpid	=  GETBIT(flags1, 15);

	fTab			= GETBIT(flags2, 0);
	fSpace			= GETBIT(flags2, 1);
	fComma			= GETBIT(flags2, 2);
	fSemiColon		= GETBIT(flags2, 3);
	fCustom			= GETBIT(flags2, 4);
	fConsecutive	= GETBIT(flags2, 5);	
	iTextDelm		= GETBITS(flags1, 6, 7);

	for (int i = 0 ; i < itwf; i++)
	{
		TxtWf val; 
		record >> val;
		rgtxtwf.push_back(val);
	}

	record >> rgchFile;
}

} // namespace XLS

