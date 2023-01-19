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

#include "Text.h"

namespace XLS
{

Text::Text()
{
	is_area			= false;
	dlp				= false;
	iReadingOrder	= 0;
	icvText			= 0xff; 
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

