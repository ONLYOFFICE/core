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
