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

#include "timeperiod.h"
#include <boost/algorithm/string.hpp>

#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const time_period & _Val)
{
    switch(_Val.get_type())
    {
	case time_period::yesterday:	_Wostream << L"yesterday"; break;
	case time_period::tomorrow:		_Wostream << L"tomorrow";	break;
	case time_period::last7Days:	_Wostream << L"last-7-days"; break;
	case time_period::thisMonth:	_Wostream << L"this-month"; break;
	case time_period::lastMonth:	_Wostream << L"last-month"; break;
	case time_period::nextMonth:	_Wostream << L"next-month"; break;
	case time_period::thisWeek:		_Wostream << L"this-week";	break;
	case time_period::lastWeek:		_Wostream << L"last-week";	break;
	case time_period::nextWeek:		_Wostream << L"next-week";	break;
    case time_period::today:		_Wostream << L"today";		break;
    default:
        _Wostream << L"today"; break;
    }
    return _Wostream;    
}

time_period time_period::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"today")
        return time_period(today);
    else if (tmp == L"yesterday")
        return time_period(yesterday);
    else if (tmp == L"tomorrow")
        return time_period(tomorrow);
    else if (tmp == L"last-7-days")
        return time_period(last7Days);
    else if (tmp == L"this-month")
        return time_period(thisMonth);
    else if (tmp == L"last-month")
        return time_period(lastMonth);
	else if (tmp == L"next-month")
		return time_period(nextMonth);
	else if (tmp == L"this-week")
		return time_period(thisWeek);
	else if (tmp == L"last-week")
		return time_period(lastWeek);
	else if (tmp == L"next-week")
		return time_period(nextWeek);
	else
    {
        return time_period(today);
    }
}

} }
