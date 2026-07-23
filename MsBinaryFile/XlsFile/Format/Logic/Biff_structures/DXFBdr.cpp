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

BiffStructurePtr DXFBdr::clone()
{
	return BiffStructurePtr(new DXFBdr(*this));
}

void DXFBdr::load(CFRecord& record)
{
	_UINT32 flags;
	record >> flags;
	
	dgLeft		= static_cast<unsigned char>(GETBITS(flags, 0, 3));
	dgRight		= static_cast<unsigned char>(GETBITS(flags, 4, 7));
	dgTop		= static_cast<unsigned char>(GETBITS(flags, 8, 11));
	dgBottom	= static_cast<unsigned char>(GETBITS(flags, 12, 15));
	
	icvLeft		= static_cast<unsigned char>(GETBITS(flags, 16, 22));
	icvRight	= static_cast<unsigned char>(GETBITS(flags, 23, 29));
	
	bitDiagDown = GETBIT(flags, 30);
	bitDiagUp	= GETBIT(flags, 31);

	record >> flags;
	
	icvTop		= static_cast<unsigned char>(GETBITS(flags, 0, 6));
	icvBottom	= static_cast<unsigned char>(GETBITS(flags, 7, 13));
	icvDiag		= static_cast<unsigned char>(GETBITS(flags, 14, 20));
	
	dgDiag		= static_cast<unsigned char>(GETBITS(flags, 21, 24));

}

void DXFBdr::save(CFRecord& record)
{
    _UINT32 flags = 0;
    SETBITS(flags, 0, 3, dgLeft)
    SETBITS(flags, 4, 7, dgRight)
    SETBITS(flags, 8, 11, dgTop)
    SETBITS(flags, 12, 15, dgBottom)

    SETBITS(flags, 16, 22, icvLeft)
    SETBITS(flags, 23, 29, icvRight)

    SETBIT(flags, 30, bitDiagDown);
    SETBIT(flags, 31, bitDiagUp);

    record << flags;
    flags = 0;

    SETBITS(flags, 0, 6, icvTop)
    SETBITS(flags, 7, 13, icvBottom)
    SETBITS(flags, 14, 20, icvDiag)
    SETBITS(flags, 21, 24, dgDiag)

    record << flags;
}

void serialize_one(std::wostream & stream, const std::wstring & name, unsigned char type,  unsigned char color, FullColorExt* colorExt)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(name)
		{
			switch(type)
			{		
				case 1: CP_XML_ATTR(L"style", L"thin");				break;
				case 2: CP_XML_ATTR(L"style", L"medium");			break;
				case 3: CP_XML_ATTR(L"style", L"dashed");			break;
				case 4: CP_XML_ATTR(L"style", L"dotted");			break;
				case 5: CP_XML_ATTR(L"style", L"thick");			break;
				case 6: CP_XML_ATTR(L"style", L"double");			break;
				case 7: CP_XML_ATTR(L"style", L"hair");				break;
				case 8: CP_XML_ATTR(L"style", L"mediumDashed");		break;
				case 9: CP_XML_ATTR(L"style", L"dashDot");			break;
				case 10: CP_XML_ATTR(L"style", L"mediumDashDot");	break;
				case 11: CP_XML_ATTR(L"style", L"dashDotDot");		break;
				case 12: CP_XML_ATTR(L"style", L"mediumDashDotDot");break;
				case 13: CP_XML_ATTR(L"style", L"slantDashDot");	break;
			}
			if (type != 0)
			{
				CP_XML_NODE(L"color")
				{			
					CP_XML_ATTR(L"indexed", color);
				}
			}
		}
	}
}

int DXFBdr::serialize(std::wostream & stream)
{
	if (parent->glTopNinch && parent->glBottomNinch && parent->glLeftNinch && parent->glRightNinch && 
		parent->glDiagDownNinch && parent->glDiagUpNinch) return 0;
 
	std::map<ExtProp::_type, ExtProp>::iterator pFind;
	FullColorExt* colorExt = NULL;

	CP_XML_WRITER(stream)
    {
		CP_XML_NODE(L"border")
		{	
			if (!parent->glLeftNinch)
			{
				if (parent->xfext)
					pFind = parent->xfext->mapRgExt.find(ExtProp::LeftBorderColor);

				colorExt = (parent->xfext && pFind != parent->xfext->mapRgExt.end()) ? &pFind->second.extPropData.color : NULL;

				serialize_one(CP_XML_STREAM(), L"left", dgLeft, icvLeft, colorExt);
			}
			if (!parent->glRightNinch)
			{
				if (parent->xfext)
					pFind = parent->xfext->mapRgExt.find(ExtProp::RightBorderColor);

				colorExt = (parent->xfext && pFind != parent->xfext->mapRgExt.end()) ? &pFind->second.extPropData.color : NULL;

				serialize_one(CP_XML_STREAM(), L"right", dgRight, icvRight, colorExt);
			}
			if (!parent->glTopNinch)
			{
				if (parent->xfext)
					pFind = parent->xfext->mapRgExt.find(ExtProp::TopBorderColor);

				colorExt = (parent->xfext && pFind != parent->xfext->mapRgExt.end()) ? &pFind->second.extPropData.color : NULL;
					
				serialize_one(CP_XML_STREAM(), L"top", dgTop, icvTop, colorExt);
			}
			if (!parent->glBottomNinch)
			{
				if (parent->xfext)
					pFind = parent->xfext->mapRgExt.find(ExtProp::BottomBorderColor);

				serialize_one(CP_XML_STREAM(), L"bottom", dgBottom, icvBottom, colorExt);
			}
			
			if (!parent->glDiagDownNinch || !parent->glDiagUpNinch)
			{
				if (parent->xfext)
					pFind = parent->xfext->mapRgExt.find(ExtProp::DiagonalBorderColor);

				colorExt = (parent->xfext && pFind != parent->xfext->mapRgExt.end()) ? &pFind->second.extPropData.color : NULL;

				serialize_one(CP_XML_STREAM(), L"diagonal", dgDiag, icvDiag, colorExt);
			}
		}
	}
	return 0;
}


} // namespace XLS

