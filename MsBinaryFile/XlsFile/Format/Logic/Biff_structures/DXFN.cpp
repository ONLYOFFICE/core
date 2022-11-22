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

#include "DXFN.h"

namespace XLS
{


BiffStructurePtr DXFN::clone()
{
	return BiffStructurePtr(new DXFN(*this));
}

void DXFN::load(CFRecord& record)
{
	dxfnum.parent = dxffntd.parent = dxfalc.parent = dxfbdr.parent = dxfpat.parent = dxfprot.parent = this;
//---------------------------------------------------
	_UINT32 flags;
	record >> flags;

	alchNinch		= GETBIT(flags, 0);
	alcvNinch		= GETBIT(flags, 1);
	wrapNinch		= GETBIT(flags, 2);
	trotNinch		= GETBIT(flags, 3);
	kintoNinch		= GETBIT(flags, 4);
	cIndentNinch	= GETBIT(flags, 5);
	fShrinkNinch	= GETBIT(flags, 6);
	fMergeCellNinch = GETBIT(flags, 7);
	
	lockedNinch		= GETBIT(flags, 8);
	hiddenNinch		= GETBIT(flags, 9);
	
	glLeftNinch		= GETBIT(flags, 10);
	glRightNinch	= GETBIT(flags, 11);
	glTopNinch		= GETBIT(flags, 12);
	glBottomNinch	= GETBIT(flags, 13);
	glDiagDownNinch = GETBIT(flags, 14);
	glDiagUpNinch	= GETBIT(flags, 15);
	
	flsNinch		= GETBIT(flags, 16);	
	icvFNinch		= GETBIT(flags, 17);
	icvBNinch		= GETBIT(flags, 18);
	
	ifmtNinch		= GETBIT(flags, 19);
	fIfntNinch		= GETBIT(flags, 20);
	
	ibitAtrNum		= GETBIT(flags, 25);
	ibitAtrFnt		= GETBIT(flags, 26);
	ibitAtrAlc		= GETBIT(flags, 27);
	ibitAtrBdr		= GETBIT(flags, 28);
	ibitAtrPat		= GETBIT(flags, 29);
	ibitAtrProt		= GETBIT(flags, 30);
	
	iReadingOrderNinch = GETBIT(flags, 31);
	
	unsigned short flags2;
	record >> flags2;
	
	fIfmtUser	= GETBIT(flags2, 0);
	fNewBorder	= GETBIT(flags2, 2);
	fZeroInited = GETBIT(flags2, 15);

	if(ibitAtrNum)
	{
		record >> dxfnum;
	}
	if(ibitAtrFnt)
	{
		record >> dxffntd;
	}
	if(ibitAtrAlc)
	{
		record >> dxfalc;
	}
	if(ibitAtrBdr)
	{
		record >> dxfbdr;
	}
	if(ibitAtrPat)
	{
		record >> dxfpat;
	}
	if(ibitAtrProt)
	{
		record >> dxfprot;
	}
}

int DXFN::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"dxf")
		{
			if(ibitAtrFnt)
			{
				dxffntd.serialize(CP_XML_STREAM());
			}
			if(ibitAtrNum)
			{
				dxfnum.serialize(CP_XML_STREAM());
			}
			if(ibitAtrPat)
			{	
				dxfpat.serialize(CP_XML_STREAM());
			}
			if(ibitAtrAlc)
			{
				dxfalc.serialize(CP_XML_STREAM());
			}
			if(ibitAtrBdr)
			{
				dxfbdr.serialize(CP_XML_STREAM());
			}
			if(ibitAtrProt)
			{
				dxfprot.serialize(CP_XML_STREAM());
			}
		}
	}

	return 0;
}

} // namespace XLS

