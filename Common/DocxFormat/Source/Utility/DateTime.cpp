#include "DateTime.h"
#include "time.h"

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


DateTime::DateTime(const CString &sValue, const CString &sPattern)	
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


const CString  DateTime::ToString  (const CString &sPattern) const
{
	CString sResult = sPattern, sTemp;

	sTemp.Format( _T("%04d"), m_nYear        ); sResult.Replace( _T("%YYYY"), sTemp ); sTemp.Empty();
	sTemp.Format( _T("%02d"), m_nMonth       ); sResult.Replace( _T("%MM"),   sTemp ); sTemp.Empty();
	sTemp.Format( _T("%02d"), m_nDay         ); sResult.Replace( _T("%DD"),   sTemp ); sTemp.Empty();
	sTemp.Format( _T("%02d"), m_nHour        ); sResult.Replace( _T("%hh"),   sTemp ); sTemp.Empty();
	sTemp.Format( _T("%02d"), m_nMinute      ); sResult.Replace( _T("%mm"),   sTemp ); sTemp.Empty();
	sTemp.Format( _T("%02d"), m_nSecond      ); sResult.Replace( _T("%ss"),   sTemp ); sTemp.Empty();
	sTemp.Format( _T("%02d"), m_nMillisecond ); sResult.Replace( _T("%ms"),   sTemp );

	return sResult;
}


const DateTime DateTime::Parse     (const CString &sValue, const CString &sPattern)
{
	return DateTime( sValue, sPattern );
}


const int      DateTime::ParseValue(const CString &sValue, const CString &sPattern, const CString &sElement)
{
	const int nPos = sPattern.Find( sElement );

	if ( -1 != nPos )
	{
		int nSepCount = 0;
		for ( int nIndex = 0; nIndex < nPos; nIndex++ )
		{
			if ( '%' == sPattern[nIndex] )
				nSepCount++;
		}

		const CString sNumeric = sValue.Mid( nPos - nSepCount , sElement.GetLength() - 1 );

		return _wtoi( sNumeric );
	}
	return 0;
}
