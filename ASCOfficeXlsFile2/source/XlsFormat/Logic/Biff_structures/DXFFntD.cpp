/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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


BiffStructurePtr DXFFntD::clone()
{
	return BiffStructurePtr(new DXFFntD(*this));
}

void DXFFntD::store(CFRecord& record)
{
	unsigned char cchFont = stFontName.getSize();
	record << cchFont;
	if(cchFont)
	{
		stFontName.setSize(cchFont);
		record << stFontName;
		record.reserveNunBytes(63 - stFontName.getStructSize()); // unused1
	}
	else
	{
		record.reserveNunBytes(63); // unused1
	}
	record << stxp << icvFore;
	record.reserveNunBytes(4); // reserved
	record << tsNinch << fSssNinch << fUlsNinch << fBlsNinch;
	record.reserveNunBytes(4); // unused1
	record << ich << cch << iFnt;
}


void DXFFntD::load(CFRecord& record)
{
	unsigned char cchFont;
	record >> cchFont;
	if(cchFont)
	{
		stFontName.setSize(cchFont);
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

int DXFFntD::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"font")
		{	
			if (!stFontName.value().empty())
			{
				CP_XML_NODE(L"name")
				{
					CP_XML_ATTR(L"val", stFontName.value());
				}
			}
			if (stxp.twpHeight > 20)
			{
				CP_XML_NODE(L"sz")
				{
					CP_XML_ATTR(L"val", stxp.twpHeight/20);
				}
			}
		   if (icvFore < 0x7fff) 
			{
				CP_XML_NODE(L"color")
				{
					CP_XML_ATTR(L"indexed", icvFore);
				}
			}
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
					switch(stxp.uls)
					{
						case 0:		CP_XML_ATTR(L"val", L"none");			break;
						case 1:		CP_XML_ATTR(L"val", L"single");			break;
						case 2:		CP_XML_ATTR(L"val", L"double");			break;
						case 33:	CP_XML_ATTR(L"val", L"singleAccounting");break;
						case 34:	CP_XML_ATTR(L"val", L"doubleAccounting");break;
					}
				}
			}
			if (fSssNinch == 0)
			{
				CP_XML_NODE(L"vertAlign")
				{
					switch(stxp.sss)
					{
						case 0:	CP_XML_ATTR(L"val", L"baseline");	break;
						case 1:	CP_XML_ATTR(L"val", L"superscript");break;
						case 2:	CP_XML_ATTR(L"val", L"subscript");	break;
					}
	               
				}
			}
		}
	}
	return 0;
}



} // namespace XLS

