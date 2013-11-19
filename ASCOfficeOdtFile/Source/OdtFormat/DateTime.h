#pragma once
#ifndef ODT_DATE_TIME_INCLUDE_H_
#define ODT_DATE_TIME_INCLUDE_H_

#include <string>
#include "Utility.h"


namespace Odt
{
	class DateTime
	{
	public:
		DateTime();
		explicit DateTime(const std::string& value);
		explicit DateTime(const ::DateTime& dt);

	public:
		const std::string ToString() const;
		static const DateTime Parse(const std::string& value);

	public:
		::DateTime& datetime();
		const ::DateTime& datetime() const;

	private:
		static const std::string s_pattern;
		::DateTime m_datetime;
	};
} // namespace Odt

#endif // ODT_DATE_TIME_INCLUDE_H_