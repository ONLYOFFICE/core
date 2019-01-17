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

#include "xlsx_utils.h"

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "../../Common/DocxFormat/Source/XML/Utils.h"

namespace cpdoccore {

namespace oox {

bool IsNumber(const std::wstring &value)
{
	boost::wregex rule(L"^\\-{0,1}[0-9]*[.,]{0,1}[0-9]*$");
	boost::match_results<std::wstring::const_iterator> results;

	return boost::regex_search(value/*.begin(), value.end(), results*/, rule);
}

std::wstring getColAddress(size_t col)
{
    static const size_t r = (L'Z' - L'A' + 1);
    std::wstring res;
    size_t r0 = col / r;

    if (r0 > 0)
    {
        const std::wstring rest = getColAddress(col - r * r0);
        const std::wstring res	= getColAddress(r0-1) + rest;
        return res;
    }
    else
        return std::wstring(1, (wchar_t)(L'A' + col));
}

std::wstring getRowAddress(size_t row)
{
    return std::to_wstring(row + 1);
}

std::wstring getCellAddress(size_t col, size_t row)
{
	return getColAddress(col) + getRowAddress(row);
}

//_ASSERTE(getColAddressInv(L"A") == 0);
//_ASSERTE(getColAddressInv(L"B") == 1);
//_ASSERTE(getColAddressInv(L"Z") == 25);
//_ASSERTE(getColAddressInv(L"AA") == 26);
//_ASSERTE(getColAddressInv(L"AZ") == 51);
//_ASSERTE(getColAddressInv(L"BA") == 52);
//_ASSERTE(getColAddressInv(L"BZ") == 77);
//_ASSERTE(getColAddressInv(L"ZZ") == 26 * 26 + 25);
size_t getColAddressInv(const std::wstring & a_)
{
    std::wstring a = a_;
    boost::algorithm::to_upper(a);
    static const size_t r = (L'Z' - L'A' + 1);
    size_t mul = 1;
    bool f = true;
    size_t res = 0;

	for (int i = a.length() - 1; i >= 0; i--)
	{
		size_t v = a[i] - L'A';
        if (f)
            f = false;
        else
            v += 1;
        res += v * mul;
        mul *= r;
    }
    return res;
}

size_t getRowAdderssInv(const std::wstring & a_)
{
	int sz = a_.length();
	if (a_.length()>0)
	{
	   return boost::lexical_cast<size_t>(a_)-1;
	}
	else
		return 0;
}

void splitCellAddress(const std::wstring & a_, std::wstring & col, std::wstring & row)
{   
	std::wstring a = a_;

	std::reverse(a.begin(), a.end());
    XmlUtils::replace_all( a, L"$", L"");
    //XmlUtils::replace_all( a, L"'", L"");
	
	boost::algorithm::to_upper(a);
	
	for (size_t i = 0; i < a.length(); i++)
	{
		if (a[i] >= L'0' && a[i] <= L'9')
			row += a[i];
		else
			col += a[i];
    }
	std::reverse(col.begin(), col.end());
	std::reverse(row.begin(), row.end());
}

//size_t colS, rowS;
//getCellAddressInv(L"A1", colS, rowS);
//_ASSERTE(colS == 0);
//_ASSERTE(rowS == 0);
//
//getCellAddressInv(L"Z2", colS, rowS);
//_ASSERTE(colS == 25);
//_ASSERTE(rowS == 1);
//
//getCellAddressInv(L"ZZ1000", colS, rowS);
//_ASSERTE(colS == 26 * 26 + 25);
//_ASSERTE(rowS == 999);
void getCellAddressInv(const std::wstring & a_, size_t & col, size_t & row)
{
    std::wstring colStr = L"", rowStr = L"";
    splitCellAddress(a_, colStr, rowStr);
    
    col = getColAddressInv( colStr );
    row = getRowAdderssInv( rowStr );
}

bool parseBoolVal(const std::wstring & str)
{
    std::wstring s = str;
    boost::algorithm::to_lower(s);
    if (L"true" == str || L"1" == str)
        return true;
    else
        return false;
}

std::wstring cellType2Str(XlsxCellType::type type)
{
    switch(type)
    {
    case XlsxCellType::b:
        return L"b";
    case XlsxCellType::n:
        return L"n";
    case XlsxCellType::e:
        return L"e";
    case XlsxCellType::s:
        return L"s";
    case XlsxCellType::str:
        return L"str";
    case XlsxCellType::inlineStr:
        return L"inlineStr";
    default:
        return L"";
    }
}

boost::int64_t convertDate(int Year, int Month, int Day)
{
	if (Year < 1400 || Year >10000)
		return - 1;
 	if (Month < 1 || Month > 12)
		return - 1;
	if (Day < 1 || Day > 31)
		return - 1;
	
	boost::int64_t daysFrom1900  =  boost::gregorian::date_duration(boost::gregorian::date(Year, Month, Day) - boost::gregorian::date(1900, 1, 1)).days() + 1;

    if (Year <= 1900 && 
        Month <= 2 &&
        Day <= 29)
    {
        return daysFrom1900;
    }
    else
    {
        return daysFrom1900 + 1;
    }
}

double convertTime(int hours, int minutes, double sec)
{  
    boost::posix_time::time_duration t(hours, minutes, 0);
    t += boost::posix_time::millisec(static_cast<boost::uint32_t>(sec * 1000));
    boost::posix_time::time_duration day(24, 0, 0);

    const boost::uint64_t m1 = t.total_milliseconds() ;
    const boost::uint64_t m2 = day.total_milliseconds() ;
    const double v = 1.0 * m1 / m2;
    return v;
}

bool parseDate(const std::wstring & Date, int & Year, int & Month, int & Day)
{
    // for example, "1899-12-31T05:37:46.665696"
    try 
    {
        boost::wregex r(L"([\\d]+)-([\\d]+)-([\\d]+)(?:T([\\d]+):([\\d]+):([\\d]+)(?:\\.([\\d]+))?)?");
        boost::match_results<std::wstring::const_iterator> res;
        if (boost::regex_match(Date, res, r))
        {
            Year    = boost::lexical_cast<int>(res[1].str());
            Month   = boost::lexical_cast<int>(res[2].str());
            Day     = boost::lexical_cast<int>(res[3].str());

            int Hours, Minutes, Sec, FSec;
            if (res[4].matched)
                Hours = boost::lexical_cast<int>(res[4].str());

            if (res[5].matched)
                Minutes = boost::lexical_cast<int>(res[5].str());

            if (res[6].matched)
                Sec = boost::lexical_cast<int>(res[6].str());

            if (res[7].matched)
                FSec = boost::lexical_cast<int>(res[7].str());

            return true;
        }
    }
    catch(...)
    {
    }
    return false;
}

bool parseTime(const std::wstring & Time, int & Hours, int & Minutes, double & seconds)
{
    try 
    {
        boost::wregex r(L"PT([\\d]+)H([\\d]+)M([\\d]+)(?:\\.([\\d]+))?S");
        boost::match_results<std::wstring::const_iterator> res;
        if (boost::regex_match(Time, res, r))
        {
            Hours   = boost::lexical_cast<int>(res[1].str());
            Minutes = boost::lexical_cast<int>(res[2].str());
            seconds = boost::lexical_cast<int>(res[3].str());

            if (res[4].matched)
            {
                seconds += boost::lexical_cast<double>(res[4].str()) / pow(10.0, 1.0 * res[4].str().length());
            }

            return true;
        }
    }
    catch(...)
    {
    }
    return false;
}

}
}
