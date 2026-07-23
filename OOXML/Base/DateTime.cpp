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
#include "time.h"
#include "DateTime.h"
#include "Unit.h"

//----------------------------------------------------------------------------------------------
//  DateTime
//----------------------------------------------------------------------------------------------
DateTime::DateTime()
{
	time_t oNow;
	tm oLocal;
	time( &oNow );

#ifdef _WIN32
	localtime_s( &oLocal, &oNow );
#else
	localtime_r( &oNow, &oLocal );
#endif

	m_nYear        = oLocal.tm_year + 1900;
	m_nMonth       = oLocal.tm_mon + 1;
	m_nDay         = oLocal.tm_mday;
	m_nHour        = oLocal.tm_hour;
	m_nMinute      = oLocal.tm_min;
	m_nSecond      = oLocal.tm_sec;
	m_nMillisecond = 0;
}


DateTime::DateTime(const std::wstring &sValue, const std::wstring &sPattern)
	:
	  m_nYear         ( ParseValue( sValue, sPattern, L"%YYYY" )),
	  m_nMonth        ( ParseValue( sValue, sPattern, L"%MM" )),
	  m_nDay          ( ParseValue( sValue, sPattern, L"%DD" )),
	  m_nHour         ( ParseValue( sValue, sPattern, L"%hh" )),
	  m_nMinute       ( ParseValue( sValue, sPattern, L"%mm" )),
	  m_nSecond       ( ParseValue( sValue, sPattern, L"%ss" )),
	  m_nMillisecond	( ParseValue( sValue, sPattern, L"%ms" ))
{
}


const std::wstring  DateTime::ToString  (const std::wstring &sPattern) const
{
	std::wstring sResult = sPattern, sTemp;

	sTemp = XmlUtils::ToString(m_nYear,   L"%04d" ); XmlUtils::replace_all(sResult, L"%YYYY", sTemp );
	sTemp = XmlUtils::ToString(m_nMonth,  L"%02d" ); XmlUtils::replace_all(sResult, L"%MM",   sTemp );
	sTemp = XmlUtils::ToString(m_nDay,    L"%02d" ); XmlUtils::replace_all(sResult, L"%DD",   sTemp );
	sTemp = XmlUtils::ToString(m_nHour,   L"%02d" ); XmlUtils::replace_all(sResult, L"%hh",   sTemp );
	sTemp = XmlUtils::ToString(m_nMinute, L"%02d" ); XmlUtils::replace_all(sResult, L"%mm",   sTemp );
	sTemp = XmlUtils::ToString(m_nSecond, L"%02d" ); XmlUtils::replace_all(sResult, L"%ss",   sTemp );
	sTemp = XmlUtils::ToString(m_nMillisecond, L"%02d" ); XmlUtils::replace_all(sResult, L"%ms", sTemp );

	return sResult;
}


const DateTime DateTime::Parse     (const std::wstring &sValue, const std::wstring &sPattern)
{
	return DateTime( sValue, sPattern );
}


const int      DateTime::ParseValue(const std::wstring &sValue, const std::wstring &sPattern, const std::wstring &sElement)
{
	const std::wstring::size_type nPos = sPattern.find( sElement );

	if (  std::wstring::npos != nPos )
	{
		int nSepCount = 0;
		for ( std::wstring::size_type nIndex = 0; nIndex < nPos; nIndex++ )
		{
			if ( '%' == sPattern[nIndex] )
				nSepCount++;
		}

		const std::wstring sNumeric = sValue.substr( nPos - nSepCount , sElement.length() - 1 );

		return XmlUtils::GetInteger( sNumeric );
	}
	return 0;
}
