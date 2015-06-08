
#include "clockvalue.h"

#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

namespace cpdoccore { namespace odf_types { 

static bool parseTime(const std::wstring & Time, double & Hours, double & Minutes, double & Seconds, int & Ms)
{
    try 
    {
		boost::match_results<std::wstring::const_iterator> res;

//Full clock values: 
//  02:30:03    = 2 hours, 30 minutes and 3 seconds
//  50:00:10.25 = 50 hours, 10 seconds and 250 milliseconds

		boost::wregex r1 (L"([\\d]+):([\\d]+):([\\d+(\\.\\d{0,})?]+)");
        if (boost::regex_match(Time, res, r1))
        {
            Hours = boost::lexical_cast<int>(res[1].str());
            Minutes = boost::lexical_cast<int>(res[2].str());
            Seconds = boost::lexical_cast<double>(res[3].str());

            return true;
        }
//Partial clock value: 
//  02:33   = 2 minutes and 33 seconds
//  00:10.5 = 10.5 seconds = 10 seconds and 500 milliseconds
		std::wstring  Time2 = L"00:10.5";
		boost::wregex r2 (L"([\\d]+):([\\d+(\\.\\d{0,})?]+)");	
        if (boost::regex_match(Time, res, r2))
        {
            Minutes = boost::lexical_cast<int>(res[1].str());
            Seconds = boost::lexical_cast<double>(res[2].str());

            return true;
        }
//Timecount values:
//  3.2h    = 3.2 hours = 3 hours and 12 minutes
//  45min   = 45 minutes
//  30s     = 30 seconds
//  5ms     = 5 milliseconds
//  12.467  = 12 seconds and 467 milliseconds
		boost::wregex r3 (L"([\\d+(\\.\\d{0,})?]+)([A-Za-z]{0,})");
        if (boost::regex_match(Time, res, r3))
        {
			if (!res[2].str().empty())
			{
				std::wstring n = res[2].str();
				std::transform(n.begin(), n.end(), n.begin(), ::tolower);
				if (n == L"h")
				{
					Hours  = boost::lexical_cast<double>(res[1].str());
				}
				else if (n == L"min")
				{
					Minutes  = boost::lexical_cast<double>(res[1].str());
				}
				else if (n == L"s")
				{
					Seconds  = boost::lexical_cast<double>(res[1].str());
				}
				else if (n == L"ms")
				{
					Ms = boost::lexical_cast<int>(res[1].str());
				}
			}
			else            
				Seconds = boost::lexical_cast<double>(res[1].str());

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
    int v=0;


	int ms=0;
	double h=0,m=0,s =0;
	bool res = parseTime(Str,h,m,s,ms);

	v = (((h*60)+m)*60+s)*1000+ms;

    return clockvalue(v);
}



} }

