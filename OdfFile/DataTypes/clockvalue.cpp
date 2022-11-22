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

#include "clockvalue.h"

#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const clockvalue & _Val)
{
 //  5ms = 5 milliseconds
	if (_Val.get_value() < 0)	
		_Wostream << L"indefinite";
	else
	{
		int ms	= _Val.get_value();
		int sec = 0;
		int min = 0;
		int h	= 0;

		if (ms > 1000)
		{
			sec = ms / 1000;
			ms	-= sec * 1000;

			if (sec > 60)
			{
				min = sec / 60;
				sec -= min * 60;

				if (min > 60)
				{
					h = min / 60;
					min -= h * 60;
				}			
			}

		}
		if ( h > 0)
			_Wostream <<  h  << "h"; 
		if ( min > 0)
			_Wostream <<  min  << "min"; 
		if ( sec > 0)
			_Wostream <<  sec  << "s"; 
		if ( ms > 0)
			_Wostream <<  ms  << "ms"; 

		if (h == 0 && min == 0 && ms == 0 && sec == 0)
			_Wostream << "0s"; 
	}
    return _Wostream;    
}

bool parseTime(std::wstring Time, double & Hours, double & Minutes, double & Seconds, int & Ms)
{
    try 
    {

//Full clock values: 
//  02:30:03    = 2 hours, 30 minutes and 3 seconds
//  50:00:10.25 = 50 hours, 10 seconds and 250 milliseconds

		boost::match_results<std::wstring::const_iterator> res1;
		boost::wregex r1 (L"([\\d]+):([\\d]+):([\\d+(\\.\\d{0,})?]+)");
        if (boost::regex_match(Time, res1, r1))
        {
            Hours = boost::lexical_cast<int>(res1[1].str());
            Minutes = boost::lexical_cast<int>(res1[2].str());
            Seconds = boost::lexical_cast<double>(res1[3].str());

            return true;
        }
//Partial clock value: 
//  02:33   = 2 minutes and 33 seconds
//  00:10.5 = 10.5 seconds = 10 seconds and 500 milliseconds
		std::wstring  Time2 = L"00:10.5";
		boost::match_results<std::wstring::const_iterator> res2;
		boost::wregex r2 (L"([\\d]+):([\\d+(\\.\\d{0,})?]+)");	
        if (boost::regex_match(Time, res2, r2))
        {
            Minutes = boost::lexical_cast<int>(res2[1].str());
            Seconds = boost::lexical_cast<double>(res2[2].str());

            return true;
        }
//Timecount values:
//  3.2h    = 3.2 hours = 3 hours and 12 minutes
//  45min   = 45 minutes
//  30s     = 30 seconds
//  5ms     = 5 milliseconds
//  12.467  = 12 seconds and 467 milliseconds
		std::vector<std::wstring> values;
		boost::wregex r3 (L"([\\d+(\\.\\d{0,})?]+)([A-Za-z]+)");
        if (boost::regex_split(std::back_inserter(values), Time, r3, boost::match_default | boost::format_all))
        {	
			int val = -1;
			for (size_t i = 0; i < values.size() ; i++ )
			{
				if (values[i].empty()) continue;

				if (values[i] == L"h")
				{
					if (val >= 0)
						Hours  = val;
					val = -1;
				}
				else if (values[i] == L"min")
				{
					if (val >= 0)
						Minutes  = val;
					val = -1;
				}
				else if (values[i] == L"s")
				{
					if (val >= 0)
						Seconds  = val;
					val = -1;
				}
				else if (values[i] == L"ms")
				{
					if (val >= 0)
						Ms  = val;
					val = -1;
				}
				else
				{
					try
					{
						val = boost::lexical_cast<double>(values[i]);
					}
					catch(...)
					{
						continue;
					}
				}
			}

            return true;
        }
    }
	catch(...)
    {
    }
    return false;
}
clockvalue clockvalue::parse(const std::wstring & Str)
{
 	if (Str == L"indefinite")
		return clockvalue(-1);
	else
	{
		int v=0;

		int ms = 0;
		double h = 0, m = 0, s = 0;
		bool res = parseTime(Str, h, m, s, ms);

		v = (((h * 60) + m) * 60 + s) * 1000 + ms;

		return clockvalue(v);
	}
}



} }

