
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "DateTime.h"


namespace Odt
{

	const std::string DateTime::s_pattern("%YYYY-%MM-%DDT%hh:%mm:%ss.%ms");

	DateTime::DateTime()
	{
	}


	DateTime::DateTime(const std::string& value)
		: m_datetime(value, s_pattern)
	{
	}


	DateTime::DateTime(const ::DateTime& dt)
		: m_datetime(dt)
	{
	}


	const std::string DateTime::ToString() const
	{
		return m_datetime.ToString(s_pattern);
	}


	const DateTime DateTime::Parse(const std::string& value)
	{
		return DateTime(value);
	}


	::DateTime& DateTime::datetime()
	{
		return m_datetime;
	}


	const ::DateTime& DateTime::datetime() const
	{
		return m_datetime;
	}

} // namespace Odt