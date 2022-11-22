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
