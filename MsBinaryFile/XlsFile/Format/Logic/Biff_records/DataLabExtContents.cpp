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

#include "DataLabExtContents.h"
#include "../Biff_structures/FrtHeader.h"
#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

DataLabExtContents::DataLabExtContents()
{
	is_area = false;
}


DataLabExtContents::~DataLabExtContents()
{
}


BaseObjectPtr DataLabExtContents::clone()
{
	return BaseObjectPtr(new DataLabExtContents(*this));
}

void DataLabExtContents::readFields(CFRecord& record)
{
	FrtHeader frtHeader;
	record >> frtHeader;

	unsigned short flags;
	record >> flags >> rgchSep;

	fSerName	= GETBIT(flags, 0);
	fCatName	= GETBIT(flags, 1);
	fValue		= GETBIT(flags, 2);
	fPercent	= GETBIT(flags, 3);
	fBubSizes	= GETBIT(flags, 4);
}

void DataLabExtContents::writeFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_DataLabExtContents);
	record << frtHeader;
	unsigned short flags = 0;
	SETBIT(flags, 0, fSerName)
	SETBIT(flags, 1, fCatName)
	SETBIT(flags, 2, fValue)
	SETBIT(flags, 3, fPercent)
	SETBIT(flags, 4, fBubSizes)
	record << flags << rgchSep;
}

int DataLabExtContents::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:showVal")
		{
			CP_XML_ATTR (L"val" , fValue); 
		}
		CP_XML_NODE(L"c:showPercent")
		{
			CP_XML_ATTR (L"val" , fPercent); 
		}
		CP_XML_NODE(L"c:showBubbleSize")
		{
			CP_XML_ATTR (L"val" , fBubSizes); 
		}

		if (rgchSep.value().empty() == false)
		{
			CP_XML_NODE(L"c:separator")
			{
				CP_XML_STREAM() << XmlUtils::EncodeXmlString(rgchSep.value());
			}
		}
		if (is_area == false)
		{
			CP_XML_NODE(L"c:showCatName")
			{
				CP_XML_ATTR (L"val", fCatName); 
			}		
			CP_XML_NODE(L"c:showSerName") {	CP_XML_ATTR (L"val" , 0); }	
		}
		else
		{
			CP_XML_NODE(L"c:showCatName") {	CP_XML_ATTR (L"val" , 0); }	
			CP_XML_NODE(L"c:showSerName")
			{
				CP_XML_ATTR (L"val" , fSerName); 
			}		
		}


	}
	return 0;
}

} // namespace XLS

