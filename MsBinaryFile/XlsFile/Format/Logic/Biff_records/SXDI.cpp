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

#include "SXDI.h"

namespace XLS
{

SXDI::SXDI()
{
}

SXDI::~SXDI()
{
}

BaseObjectPtr SXDI::clone()
{
	return BaseObjectPtr(new SXDI(*this));
}

void SXDI::readFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();
	record >> isxvdData >> iiftab >> df >> isxvd >> isxvi >> ifmt >> cchName;

	global_info->mapUsedFormatCode.insert(std::make_pair(ifmt, true));
	
	if (cchName > 0 && cchName < 0xFFFF)
	{
		stName.setSize(cchName);
		record >> stName;
	}
}

int SXDI::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"dataField")
		{
			if (stName.value().empty() == false)
				CP_XML_ATTR(L"name", stName.value()); 
			
			CP_XML_ATTR(L"fld",			isxvdData); 
			CP_XML_ATTR(L"baseField",	df); 
			CP_XML_ATTR(L"baseItem",	isxvi); 
			CP_XML_ATTR(L"numFmtId",	ifmt); 
			switch(iiftab)
			{
			case 0x0000: CP_XML_ATTR(L"subtotal", L"sum");		break;
			case 0x0001: CP_XML_ATTR(L"subtotal", L"count");	break;
			case 0x0002: CP_XML_ATTR(L"subtotal", L"average");	break;
			case 0x0003: CP_XML_ATTR(L"subtotal", L"max");		break;
			case 0x0004: CP_XML_ATTR(L"subtotal", L"min");		break;
			case 0x0005: CP_XML_ATTR(L"subtotal", L"product");	break;
			case 0x0006: CP_XML_ATTR(L"subtotal", L"countNums");break;
			case 0x0007: CP_XML_ATTR(L"subtotal", L"stdDev");	break;
			case 0x0008: CP_XML_ATTR(L"subtotal", L"stdDevp");	break;
			case 0x0009: CP_XML_ATTR(L"subtotal", L"var");		break;
			case 0x000a: CP_XML_ATTR(L"subtotal", L"varp");		break;
			}
		}
	}
	return 0;
}
} // namespace XLS

