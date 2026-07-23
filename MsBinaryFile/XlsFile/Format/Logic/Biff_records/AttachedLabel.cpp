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
//
#include "AttachedLabel.h"

namespace XLS
{

AttachedLabel::AttachedLabel()
{
	is_area = false;
}


AttachedLabel::~AttachedLabel()
{
}


BaseObjectPtr AttachedLabel::clone()
{
	return BaseObjectPtr(new AttachedLabel(*this));
}


void AttachedLabel::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	
	fShowValue			= GETBIT(flags, 0);
	fShowPercent		= GETBIT(flags, 1);
	fShowLabelAndPerc	= GETBIT(flags, 2);
	fShowLabel			= GETBIT(flags, 4);
	fShowBubbleSizes	= GETBIT(flags, 5);
	fShowSeriesName		= GETBIT(flags, 6);
}

void AttachedLabel::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fShowValue)
	SETBIT(flags, 1, fShowPercent)
	SETBIT(flags, 2, fShowLabelAndPerc)
	SETBIT(flags, 4, fShowLabel)
	SETBIT(flags, 5, fShowBubbleSizes)
	SETBIT(flags, 6, fShowSeriesName)
	record << flags;
}

int AttachedLabel::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:showVal")
		{
			CP_XML_ATTR (L"val" , fShowValue); 
		}
		CP_XML_NODE(L"c:showPercent")
		{
			CP_XML_ATTR (L"val" , fShowPercent); 
		}
		CP_XML_NODE(L"c:showBubbleSize")
		{
			CP_XML_ATTR (L"val" , fShowBubbleSizes); 
		}

		if (is_area == false)
		{
			CP_XML_NODE(L"c:showCatName")	
			{
				CP_XML_ATTR (L"val" ,fShowLabel); 
			}
			CP_XML_NODE(L"c:showSerName") {	CP_XML_ATTR (L"val" , 0); }	
		}
		else
		{
			CP_XML_NODE(L"c:showCatName") {	CP_XML_ATTR (L"val" , 0); }	
			CP_XML_NODE(L"c:showSerName")
			{
				CP_XML_ATTR (L"val" , fShowSeriesName || fShowLabel); 
			}			
		}
	}
	return 0;
}

} // namespace XLS

