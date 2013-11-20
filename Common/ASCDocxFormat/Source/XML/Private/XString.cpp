
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XString.h"
#include <boost/lexical_cast.hpp>
#include "Encoding.h"


namespace XML
{
	namespace Private
	{

		XString::XString()
		{
		}


		XString::XString(const char* value)
			: m_value(value)
		{
		}


		XString::XString(const std::string& value)
			: m_value(value)
		{
		}


		const bool XString::operator ==(const XString& rhs) const
		{
			return m_value == rhs.m_value;
		}


		const bool XString::operator !=(const XString& rhs) const
		{
			return m_value != rhs.m_value;
		}


		const bool XString::operator ==(const std::string& rhs) const
		{
			return m_value.get_value_or("") == rhs;
		}


		const bool XString::operator !=(const std::string& rhs) const
		{
			return m_value.get_value_or("") != rhs;
		}


		const bool XString::operator ==(const char* rhs) const
		{
			return m_value.get_value_or("") == rhs;
		}


		const bool XString::operator !=(const char* rhs) const
		{
			return m_value.get_value_or("") != rhs;
		}


		const bool XString::ToBool() const
		{
			if (!m_value.is_init())
				return false;
			if (m_value == "true" || m_value == "t" || m_value == "1")
				return true;
			return false;
		}


		const int XString::ToInt() const
		{
			//return boost::lexical_cast<int>(m_value.get_value_or("0"));
			try
			{
				std::string str = m_value.get_value_or("0");
				return atoi(str.c_str());
			}
			catch(...)
			{
			}
			return 0;
		}


		const size_t XString::ToSizet() const
		{
			//return boost::lexical_cast<size_t>(m_value.get_value_or("0"));
			try
			{
				std::string str = m_value.get_value_or("0");
				return (size_t)atoi(str.c_str());
			}
			catch(...)
			{
			}
			return 0;
		}


		const double XString::ToDouble() const
		{
			//return boost::lexical_cast<double>(m_value.get_value_or("0"));
			std::string str = m_value.get_value_or("0");
			double d = 0;
			try
			{
				sscanf(str.c_str(), "%lf", &d);
			}
			catch(...)
			{
				d = 0;
			}
			return d;
		}


		const std::string XString::ToString() const
		{
			return m_value.get_value_or_default();
		}


		const std::wstring XString::ToWString() const
		{
			return Encoding::utf82unicode(m_value.get_value_or_default());
		}


		const boost::filesystem::path XString::ToPath() const
		{
			return m_value.get_value_or_default();
		}


		const boost::filesystem::wpath XString::ToWPath() const
		{
			return Encoding::utf82unicode(m_value.get_value_or_default());
		}


		XString::operator const bool() const
		{
			return ToBool();
		}


		XString::operator const int() const
		{
			return ToInt();
		}


		XString::operator const size_t() const
		{
			return ToSizet();
		}


		XString::operator const double() const
		{
			return ToDouble();
		}


		XString::operator const std::string() const
		{
			return ToString();
		}


		XString::operator const std::wstring() const
		{
			return ToWString();
		}

	} // namespace Private
} // namespace XML