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

#include "LineFormat.h"

namespace XLS
{

LineFormat::LineFormat()
{
}


LineFormat::~LineFormat()
{
}


BaseObjectPtr LineFormat::clone()
{
	return BaseObjectPtr(new LineFormat(*this));
}

void LineFormat::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> rgb >> lns >> we >> flags >> icv;
	
	fAuto = GETBIT(flags, 0);
	fAxisOn = GETBIT(flags, 2);
	fAutoCo = GETBIT(flags, 3);
}

void LineFormat::writeFields(CFRecord& record)
{
	unsigned short flags = 0;

	SETBIT(flags, 0, fAuto)
	SETBIT(flags, 2, fAxisOn)
	SETBIT(flags, 3, fAutoCo)
	record << rgb << lns << we << flags << icv;
}

int LineFormat::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"a:ln")
		{
			switch(we)
			{
				case 0xFFFF:	CP_XML_ATTR(L"w", 3175);	break; //Hairline
				case 0:			CP_XML_ATTR(L"w", 12700);	break; //single
				case 1:			CP_XML_ATTR(L"w", 25400);	break; //double
				case 2:			CP_XML_ATTR(L"w", 38100);	break; //triple
			}
			if (lns == (_UINT16) 5)
			{
				CP_XML_NODE(L"a:noFill");
			}
			else
			{
				CP_XML_NODE(L"a:solidFill")
				{
					CP_XML_NODE(L"a:srgbClr")
					{
						CP_XML_ATTR(L"val",  rgb.strRGB);		
					}
				}

				CP_XML_NODE(L"a:prstDash")
				{			
					switch(lns)
					{
						case 1: CP_XML_ATTR(L"val", L"dash");			break;  //Dash
						case 2: CP_XML_ATTR(L"val", L"dot");			break;  //Dot
						case 3: CP_XML_ATTR(L"val", L"dashDot");		break;  //Dash-dot
						case 4: CP_XML_ATTR(L"val", L"sysDashDotDot");	break;  //Dash dot-dot
						case 0: 
						default:CP_XML_ATTR(L"val", L"solid");			break;  //Solid
					}
				}			
				switch(lns)
				{
					case 6: break; //Dark gray pattern
					case 7: break; //Medium gray pattern
					case 8: break; //Light gray patternn
				}
			}
		}
	}
	return 0;
}
void LineFormat::setChecksum(std::vector<BYTE> &ChecksumData)
{
	{
		BYTE icvCheck = icv;
		ChecksumData.push_back(icvCheck);
	}
	{
		BYTE lnsCheck = lns;
		ChecksumData.push_back(lnsCheck);
	}
	{
		BYTE weCheck = we+1;
		ChecksumData.push_back(weCheck);
	}
	{
		BYTE autoCheck = 0;
		SETBIT(autoCheck,0,fAuto);
		ChecksumData.push_back(autoCheck);
	}
	{
		BYTE red = rgb.red;
		ChecksumData.push_back(red);
		BYTE green = rgb.green;
		ChecksumData.push_back(green);
		BYTE Blue = rgb.blue;
		ChecksumData.push_back(Blue);
		BYTE reserved = 0;
		ChecksumData.push_back(reserved);
	}
}

} // namespace XLS

