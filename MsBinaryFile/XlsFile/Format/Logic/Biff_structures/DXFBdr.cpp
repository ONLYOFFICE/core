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

