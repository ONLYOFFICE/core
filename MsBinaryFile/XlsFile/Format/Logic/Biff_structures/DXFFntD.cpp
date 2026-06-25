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

#include "../../../../../OOXML/PPTXFormat/Theme.h"

namespace XLS
{


BiffStructurePtr DXFFntD::clone()
{
	return BiffStructurePtr(new DXFFntD(*this));
}

DXFFntD::DXFFntD()
{
    stFontName = L"";
}

void DXFFntD::load(CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();

	unsigned char cchFont;
	record >> cchFont;
	if(cchFont)
	{
		stFontName.setSize(cchFont);
		stFontName.bDeleteZero = true;
		//5804543.xls - font name in dx for table - c a l i 0 0 0 0 b r i - !!!!
		
		record >> stFontName;
		record.skipNunBytes(63 - stFontName.getStructSize()); // unused1
	}
	else
	{
		record.skipNunBytes(63); // unused1
	}
	record >> stxp >> icvFore;
	record.skipNunBytes(4); // reserved
	record >> tsNinch >> fSssNinch >> fUlsNinch >> fBlsNinch;
	record.skipNunBytes(4); // unused1
	record >> ich >> cch >> iFnt;
}

void DXFFntD::save(CFRecord& record)
{

    unsigned char cchFont = stFontName.getSize();
    record << cchFont;
    if(cchFont)
        record << stFontName;
    record.reserveNunBytes(63 - cchFont);
    record << stxp << icvFore;
    record.reserveNunBytes(4); // reserved
    record << tsNinch << fSssNinch << fUlsNinch << fBlsNinch;
    record.reserveNunBytes(4); // reserved
    record << ich << cch << iFnt;
}

int DXFFntD::serialize(std::wostream & stream, bool extOnly)
{
	std::map<ExtProp::_type, ExtProp>::iterator pFind;
	
	if (parent && parent->xfext)
		pFind = parent->xfext->mapRgExt.find(ExtProp::FontScheme);
	
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"font")
		{
			std::wstring name = stFontName.value();

			if (parent && parent->xfext)
			{
				BYTE font_scheme = (pFind != parent->xfext->mapRgExt.end()) ? pFind->second.extPropData.font_scheme : 0;

				if (global_info && global_info->m_pTheme && font_scheme == 0x01)
				{
					name = global_info->m_pTheme->themeElements.fontScheme.majorFont.latin.typeface;
				}
				else if (global_info && global_info->m_pTheme && font_scheme == 0x02)
				{
					name = global_info->m_pTheme->themeElements.fontScheme.minorFont.latin.typeface;
				}
			}

			if (!name.empty())
			{
				CP_XML_NODE(L"name")
				{
					CP_XML_ATTR(L"val", name.substr(0, 31));
				}
			}

			if (!extOnly && stxp.twpHeight > 20)
			{
				CP_XML_NODE(L"sz")
				{
					CP_XML_ATTR(L"val", stxp.twpHeight/20.f);
				}
			}
			if ((parent && parent->xfext) && (pFind == parent->xfext->mapRgExt.end()))
				pFind = parent->xfext->mapRgExt.find(ExtProp::TextColor);

			if ((parent && parent->xfext) && pFind != parent->xfext->mapRgExt.end())
			{
				pFind->second.extPropData.color.serialize(CP_XML_STREAM(), L"color");
			}
			else if (icvFore < 0x7fff) 
			{
				CP_XML_NODE(L"color")
				{
					CP_XML_ATTR(L"indexed", icvFore);
				}
			}
			if (!extOnly)
			{
				CP_XML_NODE(L"charset")
				{
					CP_XML_ATTR(L"val", stxp.bCharSet);
				}
				//CP_XML_NODE(L"condense")
				//{
				//	CP_XML_ATTR(L"val", 1);
				//}
				//CP_XML_NODE(L"extend")
				//{
				//	CP_XML_ATTR(L"val", stxp.fExtend);
				//}
				CP_XML_NODE(L"family")
				{
					CP_XML_ATTR(L"val", stxp.bFamily);
				}
				if (tsNinch.ftsItalic == 0)
				{
					CP_XML_NODE(L"i")
					{
						CP_XML_ATTR(L"val", stxp.ts.ftsItalic);
					}
				}
				if (fBlsNinch == 0)
				{
					CP_XML_NODE(L"b")
					{
						CP_XML_ATTR(L"val", stxp.bls == 700 ? 1 : 0);
					}
				}
				if (tsNinch.ftsStrikeout == 0)
				{
					CP_XML_NODE(L"strike")
					{
						CP_XML_ATTR(L"val", stxp.ts.ftsStrikeout);
					}
				}
				if (fUlsNinch == 0)
				{
					CP_XML_NODE(L"u")
					{
						switch (stxp.uls)
						{
						case 0:		CP_XML_ATTR(L"val", L"none");			break;
						case 1:		CP_XML_ATTR(L"val", L"single");			break;
						case 2:		CP_XML_ATTR(L"val", L"double");			break;
						case 33:	CP_XML_ATTR(L"val", L"singleAccounting"); break;
						case 34:	CP_XML_ATTR(L"val", L"doubleAccounting"); break;
						}
					}
				}
				if (fSssNinch == 0)
				{
					CP_XML_NODE(L"vertAlign")
					{
						switch (stxp.sss)
						{
						case 0:	CP_XML_ATTR(L"val", L"baseline");	break;
						case 1:	CP_XML_ATTR(L"val", L"superscript"); break;
						case 2:	CP_XML_ATTR(L"val", L"subscript");	break;
						}

					}
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

