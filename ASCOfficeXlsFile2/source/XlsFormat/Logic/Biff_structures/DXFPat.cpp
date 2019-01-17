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
#include <Binary/CFRecord.h>


namespace XLS
{


BiffStructurePtr DXFPat::clone()
{
	return BiffStructurePtr(new DXFPat(*this));
}

void DXFPat::load(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	
	fls = GETBITS(flags, 10, 15);
	record >> flags;
	
	icvForeground = GETBITS(flags, 0, 6);
	icvBackground = GETBITS(flags, 7, 13);
}

int DXFPat::serialize(std::wostream & stream)
{
	if (parent->flsNinch && parent->icvBNinch && parent->icvFNinch) return 0;
	
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"fill")
		{	
			CP_XML_NODE(L"patternFill")
			{
				if (!parent->flsNinch)
				{
					switch(fls)
					{		
						case 2: CP_XML_ATTR(L"patternType", L"pct50");					break; //50% gray
						case 3: CP_XML_ATTR(L"patternType", L"pct75");					break; //75% gray
						case 4: CP_XML_ATTR(L"patternType", L"pct25");					break; //25% gray
						case 5: CP_XML_ATTR(L"patternType", L"horzStripe");				break; //Horizontal stripe
						case 6: CP_XML_ATTR(L"patternType", L"vertStripe");				break; //Vertical stripe
						case 7: CP_XML_ATTR(L"patternType", L"reverseDiagStripe");		break; //Reverse diagonal stripe
						case 8: CP_XML_ATTR(L"patternType", L"diagStripe");				break; //Diagonal stripe
						case 9: CP_XML_ATTR(L"patternType", L"diagCross");				break; //Diagonal crosshatch
						case 10: CP_XML_ATTR(L"patternType", L"trellis");				break; //Thick Diagonal crosshatch
						case 11: CP_XML_ATTR(L"patternType", L"thinHorzStripe");		break; //Thin horizontal stripe
						case 12: CP_XML_ATTR(L"patternType", L"thinVertStripe");		break; //Thin vertical stripe
						case 13: CP_XML_ATTR(L"patternType", L"thinReverseDiagStripe");	break; //Thin reverse diagonal stripe
						case 14: CP_XML_ATTR(L"patternType", L"thinDiagStripe");		break; //Thin diagonal stripe
						case 15: CP_XML_ATTR(L"patternType", L"thinHorzCross");			break; //Thin horizontal crosshatch
						case 16: CP_XML_ATTR(L"patternType", L"thinDiagCross");			break; //Thin diagonal crosshatch
						case 17: CP_XML_ATTR(L"patternType", L"gray125");				break; //12.5% gray
						case 18: CP_XML_ATTR(L"patternType", L"gray0625");				break; //6.25% gray
						default:
							CP_XML_ATTR(L"patternType", L"solid");
					}
				}
				if (!parent->icvFNinch)
				{

					CP_XML_NODE(L"fgColor")
					{				
						CP_XML_ATTR(L"indexed", icvForeground);
					}
				}	
				if (!parent->icvBNinch)
				{
					CP_XML_NODE(L"bgColor")
					{				
						CP_XML_ATTR(L"indexed", icvBackground);
					}
				}

			}
		}
	}
	return 0;
}



} // namespace XLS

