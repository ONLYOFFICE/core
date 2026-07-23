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

#include "AxcExt.h"

namespace XLS
{

AxcExt::AxcExt()
{
}


AxcExt::~AxcExt()
{
}


BaseObjectPtr AxcExt::clone()
{
	return BaseObjectPtr(new AxcExt(*this));
}


void AxcExt::readFields(CFRecord& record)
{
	unsigned char flags, reserved;
	record >> catMin >> catMax >> catMajor >> duMajor >> catMinor >> duMinor >> duBase >> catCrossDate >> flags >> reserved;

	fAutoMin	= GETBIT(flags, 0);
	fAutoMax	= GETBIT(flags, 1);
	fAutoMajor	= GETBIT(flags, 2);
	fAutoMinor	= GETBIT(flags, 3);
	
	fDateAxis	= GETBIT(flags, 4);
	fAutoBase	= GETBIT(flags, 5);
	fAutoCross	= GETBIT(flags, 6);
	fAutoDate	= GETBIT(flags, 7);
}

void AxcExt::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fAutoMin)
	SETBIT(flags, 1, fAutoMax)
	SETBIT(flags, 2, fAutoMajor)
	SETBIT(flags, 3, fAutoMinor)

	SETBIT(flags, 4, fDateAxis)
	SETBIT(flags, 5, fAutoBase)
	SETBIT(flags, 6, fAutoCross)
	SETBIT(flags, 7, fAutoDate)
	record << catMin << catMax << catMajor << duMajor << catMinor << duMinor << duBase << catCrossDate << flags;
}

int AxcExt::serialize(std::wostream & _stream)
{
	if (fDateAxis == false) return 0;
	
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:auto")	{  CP_XML_ATTR(L"val", !fAutoCross); }
		
		if (fAutoMax == false)
		{
			CP_XML_NODE(L"c:max") 
			{ 
				CP_XML_ATTR(L"val", catMax);
			}
		}			
		if (fAutoMin == false)
		{
			CP_XML_NODE(L"c:min") 
			{
				CP_XML_ATTR(L"val", catMin); 
			}
		}
		std::wstring DateUnit[] = {L"days", L"months", L"years"};
		
		if ( fAutoMajor == false )
		{
			CP_XML_NODE(L"c:majorUnit")
			{
				CP_XML_ATTR(L"val", catMajor);
			}
		} 
		if ( fAutoMinor == false )
		{
			CP_XML_NODE(L"c:minorUnit")
			{
				CP_XML_ATTR(L"val", catMinor);
			}
		}	
		if ( fAutoBase == false && duBase < 3)
		{
			CP_XML_NODE(L"c:baseTimeUnit")
			{
				CP_XML_ATTR(L"val", DateUnit[duBase]);
			}
		}
		if ( duMajor < 3)
		{
			CP_XML_NODE(L"c:majorTimeUnit")
			{
				CP_XML_ATTR(L"val", DateUnit[duMajor]);
			}
		}
		if (duMinor < 3)
		{
			CP_XML_NODE(L"c:minorTimeUnit")
			{
				CP_XML_ATTR(L"val", DateUnit[duMinor]);
			}
		}
	}
	return 0;
}

} // namespace XLS

