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

