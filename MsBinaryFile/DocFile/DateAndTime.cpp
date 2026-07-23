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

#include "DateAndTime.h"

namespace DocFileFormat
{  
	DateAndTime::DateAndTime()
	{
		setDefaultValues();
	}
	DateAndTime::DateAndTime( _UINT32 val )
	{
		unsigned char* bytes = ((unsigned char*) &val);

		minutes	= (short)FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, 4 ), 0, 6 );
		hour	= (short)FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, 4 ), 6, 5 );
		day		= (short)FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, 4 ), 11, 5 );
		month	= (short)FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, 4 ), 16, 4 );
		year	= (short)( 1900 + FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, 4 ), 20, 9 ) );
		weekday	= (short)FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, 4 ), 29, 3 );
	}
	DateAndTime::DateAndTime( unsigned char* bytes, int size )
	{
		if ( size == 4 )
		{
		  minutes	= (short)FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, size ), 0, 6 );
		  hour		= (short)FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, size ), 6, 5 );
		  day		= (short)FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, size ), 11, 5 );
		  month		= (short)FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, size ), 16, 4 );
		  year		= (short)( 1900 + FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, size ), 20, 9 ) );
		  weekday	= (short)FormatUtils::GetIntFromBits( FormatUtils::BytesToInt32( bytes, 0, size ), 29, 3 );
		}
		else
		{
		  //throw new ByteParseException("Cannot parse the struct DTTM, the length of the struct doesn't match");
		}
	}
	DateAndTime& DateAndTime::operator=(const DateAndTime& oSrc)
	{
		minutes = oSrc.minutes;
		hour = oSrc.hour;
		day = oSrc.day;
		month = oSrc.month;
		year = oSrc.year;
		weekday = oSrc.weekday;

		return (*this);
	}
	std::wstring DateAndTime::getString()
	{
		return std::to_wstring(year) + L"-"	+	(month < 9 ? L"0" : L"" )	+ std::to_wstring(month)	+ L"-" +
												(day < 9 ? L"0" : L"" )		+ std::to_wstring(day)		+ L"T" +
												(hour < 9 ? L"0" : L"" )	+ std::to_wstring(hour)		+ L":" +
												(minutes < 9 ? L"0" : L"" )	+ std::to_wstring(minutes)	+ L":00Z";
	}

	void DateAndTime::setDefaultValues()
	{
	  day = 0;
	  hour = 0;
	  minutes = 0;
	  month = 0;
	  weekday = 0;
	  year = 0;
	}
}
