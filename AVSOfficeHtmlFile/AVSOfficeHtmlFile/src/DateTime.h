#pragma once
#ifndef DATE_TIME_INCLUDE_H_
#define DATE_TIME_INCLUDE_H_

#include <string>

namespace DATA_TIME
{
	class DateTime
	{
	public:
		DateTime();
		DateTime(const std::string& value, const std::string& pattern);

	public:
		const std::string ToString(const std::string& pattern) const;
		const std::string ToString() const;

	public:
		static const DateTime Parse(const std::string& value, const std::string& pattern);

	private:
		static const int ParseValue(const std::string& value, const std::string& pattern, const std::string& element);

	private:
		int m_year;
		int m_month;
		int m_day;
		int m_hour;
		int m_minute;
		int m_second;
		int m_millisecond;
	};
}

#endif // DATE_TIME_INCLUDE_H_