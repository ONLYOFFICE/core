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
#include "DateTime.h"
#include "time.h"
#include "../XML/Utils.h"

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
    m_nYear         ( ParseValue( sValue, sPattern, _T("%YYYY")) ),
    m_nMonth        ( ParseValue( sValue, sPattern, _T("%MM")  ) ),
    m_nDay          ( ParseValue( sValue, sPattern, _T("%DD")  ) ),
    m_nHour         ( ParseValue( sValue, sPattern, _T("%hh")  ) ),
    m_nMinute       ( ParseValue( sValue, sPattern, _T("%mm")  ) ),
    m_nSecond       ( ParseValue( sValue, sPattern, _T("%ss")  ) ),
    m_nMillisecond	( ParseValue( sValue, sPattern, _T("%ms")  ) )
{
}


const std::wstring  DateTime::ToString  (const std::wstring &sPattern) const
{
    std::wstring sResult = sPattern, sTemp;

    sTemp = XmlUtils::IntToString(m_nYear,  _T("%04d") ); XmlUtils::replace_all(sResult, _T("%YYYY"), sTemp );
    sTemp = XmlUtils::IntToString(m_nMonth, _T("%02d") ); XmlUtils::replace_all(sResult, _T("%MM"),   sTemp );
    sTemp = XmlUtils::IntToString(m_nDay,   _T("%02d") ); XmlUtils::replace_all(sResult, _T("%DD"),   sTemp );
    sTemp = XmlUtils::IntToString(m_nHour,  _T("%02d") ); XmlUtils::replace_all(sResult, _T("%hh"),   sTemp );
    sTemp = XmlUtils::IntToString(m_nMinute, _T("%02d") ); XmlUtils::replace_all(sResult, _T("%mm"),   sTemp );
    sTemp = XmlUtils::IntToString(m_nSecond, _T("%02d") ); XmlUtils::replace_all(sResult, _T("%ss"),   sTemp );
    sTemp = XmlUtils::IntToString(m_nMillisecond, _T("%02d") ); XmlUtils::replace_all(sResult, _T("%ms"),   sTemp );

	return sResult;
}


const DateTime DateTime::Parse     (const std::wstring &sValue, const std::wstring &sPattern)
{
	return DateTime( sValue, sPattern );
}


const int      DateTime::ParseValue(const std::wstring &sValue, const std::wstring &sPattern, const std::wstring &sElement)
{
    const int nPos = sPattern.find( sElement );

	if ( -1 != nPos )
	{
		int nSepCount = 0;
		for ( int nIndex = 0; nIndex < nPos; nIndex++ )
		{
			if ( '%' == sPattern[nIndex] )
				nSepCount++;
		}

        const std::wstring sNumeric = sValue.substr( nPos - nSepCount , sElement.length() - 1 );

        return _wtoi( sNumeric.c_str() );
	}
	return 0;
}
