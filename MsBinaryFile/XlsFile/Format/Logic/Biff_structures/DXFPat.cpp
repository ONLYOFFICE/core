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
std::wstring GetPatternType(unsigned char fls)
{
	std::wstring res;
	switch (fls)
	{
	case 2: res =  L"pct50";					break; //50% gray
	case 3: res =  L"pct75";					break; //75% gray
	case 4: res =  L"pct25";					break; //25% gray
	case 5: res =  L"horzStripe";				break; //Horizontal stripe
	case 6: res =  L"vertStripe";				break; //Vertical stripe
	case 7: res =  L"reverseDiagStripe";		break; //Reverse diagonal stripe
	case 8: res =  L"diagStripe";				break; //Diagonal stripe
	case 9: res =  L"diagCross";				break; //Diagonal crosshatch
	case 10: res =  L"trellis";					break; //Thick Diagonal crosshatch
	case 11: res =  L"thinHorzStripe";			break; //Thin horizontal stripe
	case 12: res =  L"thinVertStripe";			break; //Thin vertical stripe
	case 13: res =  L"thinReverseDiagStripe";	break; //Thin reverse diagonal stripe
	case 14: res =  L"thinDiagStripe";			break; //Thin diagonal stripe
	case 15: res =  L"thinHorzCross";			break; //Thin horizontal crosshatch
	case 16: res =  L"thinDiagCross";			break; //Thin diagonal crosshatch
	case 17: res =  L"gray125";					break; //12.5% gray
	case 18: res =  L"gray0625";				break; //6.25% gray
	default:
		res =  L"solid";
	}
	return res;
}
int DXFPat::serialize(std::wostream & stream)
{
	if (parent->flsNinch && parent->icvBNinch && parent->icvFNinch) return 0;

	CP_XML_WRITER(stream)
	{
		CP_XML_NODE(L"fill")
		{
			std::map<ExtProp::_type, ExtProp>::iterator pFindGradient;
			std::map<ExtProp::_type, ExtProp>::iterator pFindBack;
			std::map<ExtProp::_type, ExtProp>::iterator pFindFore;

			if (parent->xfext)
			{
				pFindGradient = parent->xfext->mapRgExt.find(ExtProp::GradientColor);
				pFindBack = parent->xfext->mapRgExt.find(ExtProp::BackColor);
				pFindFore = parent->xfext->mapRgExt.find(ExtProp::GradientColor);
			}
			if (parent->xfext && pFindGradient != parent->xfext->mapRgExt.end())
			{
				pFindGradient->second.extPropData.gradient_fill.serialize(CP_XML_STREAM());
			}
			else
			{
				CP_XML_NODE(L"patternFill")
				{
					if (!parent->flsNinch)
					{
						CP_XML_ATTR(L"patternType", GetPatternType(fls));
					}

					if (!parent->icvFNinch)
					{
						if (parent->xfext && pFindFore != parent->xfext->mapRgExt.end())
							pFindFore->second.extPropData.color.serialize(CP_XML_STREAM(), L"fgColor");
						else
						{
							CP_XML_NODE(L"fgColor")
							{
								CP_XML_ATTR(L"indexed", icvForeground);
							}
						}
					}
					if (!parent->icvBNinch)
					{
						if (parent->xfext && pFindBack != parent->xfext->mapRgExt.end())
							pFindBack->second.extPropData.color.serialize(CP_XML_STREAM(), L"bgColor");
						else
						{
							CP_XML_NODE(L"bgColor")
							{
								CP_XML_ATTR(L"indexed", icvBackground);
							}
						}
					}
				}
			}
		}
	}
	return 0;
}



} // namespace XLS

