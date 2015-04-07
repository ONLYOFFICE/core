#pragma once
#ifndef UTILITY_DATE_TIME_INCLUDE_H_
#define UTILITY_DATE_TIME_INCLUDE_H_

#include "../Base/Base.h"

//----------------------------------------------------------------------------------------------
//  DateTime
//----------------------------------------------------------------------------------------------

class DateTime
{
public:
	DateTime();
	DateTime(const CString& sValue, const CString &sPattern);

public:

	const CString ToString(const CString &sPattern) const;

	static const DateTime Parse     (const CString &sValue, const CString &sPattern);
	static const int      ParseValue(const CString &sValue, const CString &sPattern, const CString &sElement);

private:

	int m_nYear;
	int m_nMonth;
	int m_nDay;
	int m_nHour;
	int m_nMinute;
	int m_nSecond;
	int m_nMillisecond;
};

#endif // UTILITY_DATE_TIME_INCLUDE_H_