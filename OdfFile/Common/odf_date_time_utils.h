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
#pragma once

#include <string>
#include <cmath>
#include <cstdint>
#include <cwctype>

#include <boost/lexical_cast.hpp>

#include "../DataTypes/officevaluetype.h"

namespace cpdoccore {
namespace odf_datetime {

//------------------------------------------------------------------ calendar arithmetic

//  days from serial 0 (1899-12-30) 
static const int64_t serial_epoch_shift = 25569;

//  Howard Hinnant's algorithm
inline int64_t days_from_civil(int y, int m, int d)
{
	y -= (m <= 2) ? 1 : 0;

	const int64_t era = (y >= 0 ? y : y - 399) / 400;
	const int64_t yoe = y - era * 400;									// [0, 399]
	const int64_t doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;	// [0, 365]
	const int64_t doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;			// [0, 146096]

	return era * 146097 + doe - 719468;
}
//  days since 1970-01-01 -> Gregorian
inline void civil_from_days(int64_t z, int & y, int & m, int & d)
{
	z += 719468;

	const int64_t era = (z >= 0 ? z : z - 146096) / 146097;
	const int64_t doe = z - era * 146097;										// [0, 146096]
	const int64_t yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;	// [0, 399]
	const int64_t doy = doe - (365 * yoe + yoe / 4 - yoe / 100);					// [0, 365]
	const int64_t mp	 = (5 * doy + 2) / 153;										// [0, 11]

	d = (int)(doy - (153 * mp + 2) / 5 + 1);									// [1, 31]
	m = (int)(mp + (mp < 10 ? 3 : -9));											// [1, 12]
	y = (int)(yoe + era * 400 + (m <= 2 ? 1 : 0));
}
inline bool is_valid_civil(int y, int m, int d)
{
	static const int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if (m < 1 || m > 12 || d < 1) return false;

	int last = days_in_month[m - 1];
	if (2 == m && ((0 == y % 4 && 0 != y % 100) || 0 == y % 400)) last = 29;

	return d <= last;
}
inline int64_t serial_from_days(int64_t days)
{
	const int64_t serial = days + serial_epoch_shift;
	return serial - (serial < 61 ? 1 : 0);	//  skip 1900-02-29
}
inline int64_t days_from_serial(int64_t serial)
{
	return serial - serial_epoch_shift + (serial < 60 ? 1 : 0);
}

//------------------------------------------------------------------ oox -> odf

inline std::wstring pad_number(int value, size_t width)
{
	std::wstring res = boost::lexical_cast<std::wstring>(value);
	return res.length() < width ? std::wstring(width - res.length(), L'0') + res : res;
}
//  serial number -> 'YYYY-MM-DD'
inline bool convert_date(int64_t date, std::wstring & date_str)
{
	int y = 0, m = 0, d = 0;
	civil_from_days(days_from_serial(date), y, m, d);

	if (y < 0 || y > 9999)
	{
		date_str = boost::lexical_cast<std::wstring>(date);
		return false;
	}
	date_str = pad_number(y, 4) + L"-" + pad_number(m, 2) + L"-" + pad_number(d, 2);
	return true;
}
//  fraction of a day -> 'hh:mm:ss[.sss]' | 'hhHmmMss[.sss]S'
inline std::wstring convert_time(double dTime, bool bPT)
{
	int64_t ms = (int64_t)std::floor(dTime * 86400000. + 0.5);
	if (ms < 0) ms = 0;

	const int hours		= (int)(ms / 3600000);
	const int minutes	= (int)(ms / 60000 % 60);
	const int seconds	= (int)(ms / 1000 % 60);
	const int millis	= (int)(ms % 1000);

	std::wstring time_str =
		pad_number(hours, 2)	+ (bPT ? std::wstring(L"H") : std::wstring(L":")) +
		pad_number(minutes, 2)	+ (bPT ? std::wstring(L"M") : std::wstring(L":")) +
		pad_number(seconds, 2);

	if (0 != millis)
	{
		std::wstring fraction = pad_number(millis, 3);
		while (false == fraction.empty() && L'0' == fraction[fraction.length() - 1])
		{
			fraction.erase(fraction.length() - 1);
		}
		time_str += L"." + fraction;
	}
	if (bPT) time_str += L"S";

	return time_str;
}

//------------------------------------------------------------------ odf -> oox

inline bool parse_number(const std::wstring & str, size_t pos, size_t count, double & value)
{
	if (str.length() < pos + count || 0 == count) return false;
	try
	{
		value = boost::lexical_cast<double>(str.substr(pos, count));
	}
	catch (...)
	{
		return false;
	}
	return true;
}
//  'hh:mm:ss[.sss]' | 'PT[hh]H[mm]M[ss[.sss]]S' -> fraction of a day
inline bool parse_time_value(const std::wstring & time_value, double & value)
{
	double hour = 0, minute = 0, second = 0;

	if (0 == time_value.find(L"PT"))
	{
		const size_t h	 = time_value.find(L'H');
		const size_t m	 = time_value.find(L'M');
		const size_t sec = time_value.find(L'S');

		if (std::wstring::npos == h && std::wstring::npos == m && std::wstring::npos == sec) return false;

		size_t pos = 2;
		if (std::wstring::npos != h)
		{
			if (false == parse_number(time_value, pos, h - pos, hour)) return false;
			pos = h + 1;
		}
		if (std::wstring::npos != m)
		{
			if (false == parse_number(time_value, pos, m - pos, minute)) return false;
			pos = m + 1;
		}
		if (std::wstring::npos != sec)
		{
			if (false == parse_number(time_value, pos, sec - pos, second)) return false;
		}
	}
	else
	{
		if (8 > time_value.length()) return false;
		if (L':' != time_value[2] || L':' != time_value[5]) return false;

		size_t count = 0;
		while (6 + count < time_value.length() &&
			(0 != std::iswdigit(time_value[6 + count]) || L'.' == time_value[6 + count])) ++count;

		if (false == parse_number(time_value, 0, 2, hour))		return false;
		if (false == parse_number(time_value, 3, 2, minute))		return false;
		if (false == parse_number(time_value, 6, count, second))	return false;
	}
	value = (hour * 3600. + minute * 60. + second) / 86400.;
	return true;
}
//  'YYYY-MM-DD[Thh:mm:ss[.sss]]' -> serial number
inline bool parse_date_value(const std::wstring & date_value, double & value)
{
	double year = 0, month = 0, day = 0;

	if (10 > date_value.length()) return false;
	if (L'-' != date_value[4] || L'-' != date_value[7]) return false;

	if (false == parse_number(date_value, 0, 4, year))	return false;
	if (false == parse_number(date_value, 5, 2, month))	return false;
	if (false == parse_number(date_value, 8, 2, day))	return false;

	const int y = (int)year, m = (int)month, d = (int)day;

	if (1900 == y && 2 == m && 29 == d)
	{	//  does not exist, but it is exactly what serial 60 means to a spreadsheet
		value = 60;
	}
	else
	{
		//  without this the civil algorithm silently rolls 2023-02-31 over to 2023-03-03
		if (false == is_valid_civil(y, m, d)) return false;

		value = (double)serial_from_days(days_from_civil(y, m, d));
	}

	const size_t pos = date_value.find(L'T');
	if (std::wstring::npos != pos)
	{
		double time = 0;
		if (false == parse_time_value(date_value.substr(pos + 1), time)) return false;
		value += time;
	}
	return true;
}

//------------------------------------------------------------------ cell values

inline bool convert_iso_date_time(const std::wstring & oox_date_time, odf_types::office_value_type::type & odf_type, std::wstring & odf_date_time)
{
	double value = 0;

	if (parse_date_value(oox_date_time, value))
	{
		odf_type		= odf_types::office_value_type::Date;
		odf_date_time	= oox_date_time;
		return true;
	}
	if (parse_time_value(oox_date_time, value))
	{
		odf_type		= odf_types::office_value_type::Time;
		odf_date_time	= L"PT" + convert_time(value, true);
		return true;
	}
	odf_date_time = oox_date_time;
	return false;
}
inline bool convert_date_time(const std::wstring & oox_date_time, odf_types::office_value_type::type & odf_type, std::wstring & odf_date_time)
{
	double dDateTime = 0;
	try
	{
		dDateTime = boost::lexical_cast<double>(oox_date_time);
	}
	catch (...)
	{
		return convert_iso_date_time(oox_date_time, odf_type, odf_date_time);
	}

	//  rounding 23:59:59.9999 
	int64_t ms = (int64_t)std::floor(dDateTime * 86400000. + 0.5);

	int64_t nDate = ms / 86400000;
	int64_t nTime = ms % 86400000;
	if (nTime < 0)
	{
		nTime += 86400000;
		--nDate;
	}
	const bool bPT = (nDate < 1);	

	std::wstring sDate, sTime;
	bool res = true;

	if (nTime > 0)
	{
		sTime = convert_time((double)nTime / 86400000., bPT);
	}
	if (false == bPT)
	{
		res = convert_date(nDate, sDate);
		odf_type = odf_types::office_value_type::Date;
	}
	else
		odf_type = odf_types::office_value_type::Time;

	// "1899-12-31T05:37:46.66569
	if (sDate.empty() && sTime.empty())
	{
		res = false;
		odf_date_time = oox_date_time;
	}
	else
	{
		odf_date_time = bPT ? L"PT" + sTime : sDate + (sTime.empty() ? L"" : L"T" + sTime);
	}
	return res;
}
//  serial number -> 'YYYY-MM-DD'
inline bool convert_date(const std::wstring & oox_date, std::wstring & odf_date)
{
	int64_t iDate = 0;
	try
	{
		iDate = boost::lexical_cast<int64_t>(oox_date);
	}
	catch (...)
	{
		odf_date = oox_date;
		return false;
	}
	return convert_date(iDate, odf_date);
}
//  fraction of a day -> 'PThhHmmMss[.sss]S'
inline std::wstring convert_time(const std::wstring & oox_time)
{
	double dTime = 0;
	try
	{
		dTime = boost::lexical_cast<double>(oox_time);
	}
	catch (...)
	{
		return oox_time;
	}
	return std::wstring(L"PT") + convert_time(dTime, true);
}

}
}
