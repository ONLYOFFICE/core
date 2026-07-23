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

void DXFPat::save(CFRecord& record)
{
    unsigned short flags = 0;
    SETBITS(flags, 10, 15, fls);
    record << flags;
    flags = 0;
    SETBITS(flags, 0, 6, icvForeground);
    SETBITS(flags, 7, 13, icvBackground);
    record << flags;
}

std::wstring GetPatternType(unsigned char fls)
{
	std::wstring res;
	switch (fls)
	{
	case 2: res =  L"mediumGray";					break; //50% gray
	case 3: res =  L"darkGray";					break; //75% gray
	case 4: res =  L"lightGray";					break; //25% gray
	case 5: res =  L"darkHorizontal";				break; //Horizontal stripe
	case 6: res =  L"darkVertical";				break; //Vertical stripe
	case 7: res =  L"darkDown";		break; //Reverse diagonal stripe
	case 8: res =  L"darkUp";				break; //Diagonal stripe
	case 9: res =  L"darkGrid";				break; //Diagonal crosshatch
	case 10: res =  L"darkTrellis";					break; //Thick Diagonal crosshatch
	case 11: res =  L"lightHorizontal";			break; //Thin horizontal stripe
	case 12: res =  L"lightVertical";			break; //Thin vertical stripe
	case 13: res =  L"lightDown";	break; //Thin reverse diagonal stripe
	case 14: res =  L"lightUp";			break; //Thin diagonal stripe
	case 15: res =  L"lightGrid";			break; //Thin horizontal crosshatch
	case 16: res =  L"lightTrellis";			break; //Thin diagonal crosshatch
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
				pFindFore = parent->xfext->mapRgExt.find(ExtProp::ForeColor);
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

