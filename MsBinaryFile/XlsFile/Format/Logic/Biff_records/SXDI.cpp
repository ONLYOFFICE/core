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

	ifmt = global_info->RegisterNumFormat(ifmt, L""); // return update
	
	if (cchName > 0 && cchName < 0xFFFF)
	{
		stName.setSize(cchName);
		record >> stName;
	}
	
	ifmt = global_info->RegisterNumFormat(ifmt, L""); // return update
}

void SXDI::writeFields(CFRecord& record)
{
    cchName = stName.getSize();
    record << isxvdData << iiftab << df << isxvd << isxvi << ifmt << cchName;
    if (cchName > 0 && cchName < 0xFFFF)
    {
        record << stName;
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

