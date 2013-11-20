
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "DateTime.h"


namespace OOX
{

	const std::string DateTime::s_pattern("%YYYY-%MM-%DDT%hh:%mm:%ssZ");

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

} // namespace OOX