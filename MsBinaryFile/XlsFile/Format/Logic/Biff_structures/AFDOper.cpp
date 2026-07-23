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

#include "AFDOper.h"

namespace XLS
{


BiffStructurePtr AFDOper::clone()
{
	return BiffStructurePtr(new AFDOper(*this));
}

AFDOper::AFDOper()
{	
	m_bAutoFilter	= true;
	vt				= 0xff;
	grbitSign		= 0xff;
}

AFDOper::AFDOper(bool bAutoFilter)
{
	m_bAutoFilter = bAutoFilter;
}


AFDOper::~AFDOper()
{
}


void AFDOper::load(CFRecord& record)
{
	record >> vt >> grbitSign;

	switch(vt)
	{
		case  0x02:
		{
			record >> vtValueRk;
		}break;
		case  0x04:
		{
			record >> vtValueNum;
		}break;
		case  0x06:
		{
			vtValueStr.m_bAutoFilter = m_bAutoFilter;
			record >> vtValueStr;
		}break;
		case  0x08:
		{
			record >> vtValueBool;
		}break;
		case 0x0C:	//All blanks are matched.
		case 0x0E:	//All non-blanks are matched.
		case 0x00:
		default:
		{
			record.skipNunBytes(8);
		}break;
	}
}

void AFDOper::save(CFRecord& record)
{
    record << vt << grbitSign;
    switch(vt)
    {
        case  0x02:
        {
            record << vtValueRk;
        }break;
        case  0x04:
        {
            record << vtValueNum;
        }break;
        case  0x06:
        {
            vtValueStr.m_bAutoFilter = m_bAutoFilter;
            record << vtValueStr;
        }break;
        case  0x08:
        {
            record << vtValueBool;
        }break;
        case 0x0C:	//All blanks are matched.
        case 0x0E:	//All non-blanks are matched.
        case 0x00:
        default:
        {
            record.reserveNunBytes(8);
        }break;
    }
}

int AFDOper::serialize(std::wostream & strm, const std::wstring &node_name, const std::wstring &val)
{
	if (grbitSign < 1 || grbitSign > 6) return 0;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(node_name)
		{
			switch(grbitSign)
			{
				case 0x0001: CP_XML_ATTR(L"operator", L"lessThan");			break;
				case 0x0002: CP_XML_ATTR(L"operator", L"equal");			break;
				case 0x0003: CP_XML_ATTR(L"operator", L"lessThanOrEqual");	break;
				case 0x0004: CP_XML_ATTR(L"operator", L"greaterThan");		break;
				case 0x0005: CP_XML_ATTR(L"operator", L"notEqual");			break;
				case 0x0006: CP_XML_ATTR(L"operator", L"greaterThanOrEqual");break;
			}
			if (false == val.empty())
			{
				CP_XML_ATTR(L"val", val);
			}
			else
			{
				switch(vt)
				{
					case  0x02:	CP_XML_ATTR(L"val", vtValueRk.rk);		break;
					case  0x04:	CP_XML_ATTR(L"val", vtValueNum.val);	break;
					case  0x06: //??
					{
						std::wstring val;
						for (unsigned char i = 0; i < vtValueStr.cch; i++) 
							if (vtValueStr.fCompare == 0) val += L"?"; else val += L"*";

						CP_XML_ATTR(L"val", val);	
					}break;
					case  0x08:	
					{
							CP_XML_ATTR(L"val", vtValueBool.bes == 1 ? L"1" : L"0"); 
					}break;	
					case 0x0C:	//All blanks are matched.
					case 0x0E:	//All non-blanks are matched.
					{
						CP_XML_ATTR(L"val", L" ");
					}break;
					case 0x00:
					default:
					{
					}break;
				}
			}
		}
	}
	return 1;
}

} // namespace XLS

