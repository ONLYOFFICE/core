/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "Text.h"

namespace XLS
{

Text::Text()
{
	is_area			= false;
	dlp				= false;
	iReadingOrder	= 0;
	icvText			= 0;
}


Text::~Text()
{
}


BaseObjectPtr Text::clone()
{
	return BaseObjectPtr(new Text(*this));
}

void Text::readFields(CFRecord& record)
{
	unsigned short flags1;
	unsigned short flags2;
	record >> at  >> vat >> wBkgMode >> rgbText >> x >> y >> dx >> dy >> flags1;
	
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		unsigned char orient = GETBITS(flags1, 8, 10);
		switch(orient)
		{
			case 0:	trot = 0;		break;		// Text orientation: not rotated.
			case 1: trot = 0xff;	break;		// Text orientation: vertically stacked.
			case 2: trot = 90;		break;		// Text orientation: 90 deg counterclockwise.
			case 3: trot = 270;		break;		// Text orientation: 90 deg clockwise.
		}
	}
	else
	{
		record >> icvText >> flags2 >> trot;
		//icv -> from Palette
		dlp				= GETBITS(flags2, 0, 3);
		iReadingOrder	= GETBITS(flags2, 14, 15);
	}
	
	fAutoColor			= GETBIT(flags1, 0);
	fShowKey			= GETBIT(flags1, 1);
	fShowValue			= GETBIT(flags1, 2);
	fAutoText			= GETBIT(flags1, 4);
	fGenerated			= GETBIT(flags1, 5);
	fDeleted			= GETBIT(flags1, 6);
	fAutoMode			= GETBIT(flags1, 7);
	fShowLabelAndPerc	= GETBIT(flags1, 11);
	fShowPercent		= GETBIT(flags1, 12);
	fShowBubbleSizes	= GETBIT(flags1, 13);
	fShowLabel			= GETBIT(flags1, 14);

}

void Text::writeFields(CFRecord& record)
{
	unsigned short flags1 = 0;
	unsigned short flags2 = 0;

	SETBIT(flags1, 0, fAutoColor)
	SETBIT(flags1, 1, fShowKey)
	SETBIT(flags1, 2, fShowValue)
	SETBIT(flags1, 4, fAutoText)
	SETBIT(flags1, 5, fGenerated)
	SETBIT(flags1, 6, fDeleted)
	SETBIT(flags1, 7, fAutoMode)
	SETBIT(flags1, 11, fShowLabelAndPerc)
	SETBIT(flags1, 12, fShowPercent)
	SETBIT(flags1, 13, fShowBubbleSizes)
	SETBIT(flags1, 14, fShowLabel)
	record << at  << vat << wBkgMode << rgbText << x << y << dx << dy << flags1;

	SETBITS(flags2, 0, 3, dlp);
	SETBITS(flags2, 14, 15, iReadingOrder);
	record << icvText << flags2 << trot;


}

int Text::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:showVal")			{	CP_XML_ATTR (L"val" , fShowValue); }
		if ( is_area )
		{
		 	CP_XML_NODE(L"c:showSerName")	{	CP_XML_ATTR (L"val" , fShowLabel); }
			CP_XML_NODE(L"c:showCatName")	{	CP_XML_ATTR (L"val" , 0); }
		}
		else
		{
		 	CP_XML_NODE(L"c:showSerName")	{	CP_XML_ATTR (L"val" , 0); }
			CP_XML_NODE(L"c:showCatName")	{	CP_XML_ATTR (L"val" , fShowLabel); }
		}
		CP_XML_NODE(L"c:showPercent")		{	CP_XML_ATTR (L"val" , fShowPercent); }
		CP_XML_NODE(L"c:showBubbleSize")	{	CP_XML_ATTR (L"val" , fShowBubbleSizes); }
	}
	return 0;
}

} // namespace XLS

