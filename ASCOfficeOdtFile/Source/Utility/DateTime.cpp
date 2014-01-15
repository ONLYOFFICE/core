
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "DateTime.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp>
#include "time.h"
#include <algorithm>
#include <boost/format.hpp>


DateTime::DateTime()
{
	time_t now;
	tm local;
	time(&now);
	localtime_s(&local, &now);

	m_year = local.tm_year + 1900;
	m_month = local.tm_mon + 1;
	m_day = local.tm_mday;
	m_hour = local.tm_hour;
	m_minute = local.tm_min;
	m_second = local.tm_sec;
	m_millisecond = 0;
}


DateTime::DateTime(const std::string& value, const std::string& pattern)	
	:
	m_year				(ParseValue(value, pattern, "%YYYY")),
	m_month				(ParseValue(value, pattern, "%MM")),
	m_day					(ParseValue(value, pattern, "%DD")),
	m_hour				(ParseValue(value, pattern, "%hh")),
	m_minute			(ParseValue(value, pattern, "%mm")),
	m_second			(ParseValue(value, pattern, "%ss")),
	m_millisecond	(ParseValue(value, pattern, "%ms"))
{
}


const std::string DateTime::ToString(const std::string& pattern) const
{
	std::string result = pattern;
	boost::replace_all(result, "%YYYY", (boost::format("%04d") % m_year).str());
	boost::replace_all(result, "%MM", (boost::format("%02d") % m_month).str());
	boost::replace_all(result, "%DD", (boost::format("%02d") % m_day).str());
	boost::replace_all(result, "%hh", (boost::format("%02d") % m_hour).str());
	boost::replace_all(result, "%mm", (boost::format("%02d") % m_minute).str());
	boost::replace_all(result, "%ss", (boost::format("%02d") % m_second).str());
	boost::replace_all(result, "%ms", (boost::format("%02d") % m_millisecond).str());
	return result;
}


const DateTime DateTime::Parse(const std::string& value, const std::string& pattern)
{
	return DateTime(value, pattern);
}


const int DateTime::ParseValue(const std::string& value, const std::string& pattern, const std::string& element)
{
	const int pos = pattern.find(element);
	if (pos != std::string::npos)
	{
		const int sepCount = std::count(pattern.begin(), pattern.begin() + pos, '%');
		const std::string numeric = value.substr(pos - sepCount , element.size() - 1);
		return boost::lexical_cast<int>(numeric);
	}
	return 0;
}