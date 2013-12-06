#pragma once
#ifndef XML_PRIVATE_XSTRING_INCLUDE_H_
#define XML_PRIVATE_XSTRING_INCLUDE_H_

#include <boost/operators.hpp>
#include <string>
#include <boost/filesystem.hpp>
#include "nullable.h"
#include "property.h"
#include "nullable_property.h"
#include "Parse.h"


namespace XML
{
	namespace Private
	{
		class XString
		{
		public:
			XString();
			XString(const char* value);
			XString(const std::string& value);

		public:
			const bool operator ==(const XString& rhs) const;
			const bool operator !=(const XString& rhs) const;
			const bool operator ==(const std::string& rhs) const;
			const bool operator !=(const std::string& rhs) const;
			const bool operator ==(const char* rhs) const;
			const bool operator !=(const char* rhs) const;

		public:
			const bool ToBool() const;
			const int ToInt() const;
			const size_t ToSizet() const;
			const double ToDouble() const;
			const std::string ToString() const;
			const std::wstring ToWString() const;
			const boost::filesystem::path ToPath() const;
			const boost::filesystem::wpath ToWPath() const;

			operator const bool() const;
			operator const int() const;
			operator const size_t() const;
			operator const double() const;
			operator const std::string() const;
			operator const std::wstring() const;

			template<typename T> 
			operator const nullable__<T>() const 
			{
				if (m_value.is_init())
					return Parse<T>(m_value);
				return nullable__<T>();
			}	
			
			template<typename T, class S, class G> 
			operator const nullable_property<T, S, G>() const 
			{
				if (m_value.is_init())
					return Parse<T>(m_value);
				return nullable_property<T, S, G>();
			}

		public:
			template<typename T> const bool operator ==(const T value) const
			{
				return static_cast<T>(*this) == value;
			}

			template<typename T> const bool operator !=(const T value) const
			{
				return static_cast<T>(*this) != value;
			}

			template<typename T> const bool operator > (const T value) const
			{
				return static_cast<T>(*this) > value;
			}

			template<typename T> const bool operator < (const T value) const
			{
				return static_cast<T>(*this) < value;
			}

			template<typename T> const bool operator >=(const T value) const
			{
				return static_cast<T>(*this) >= value;
			}

			template<typename T> const bool operator <=(const T value) const
			{
				return static_cast<T>(*this) <= value;
			}

			template<typename T, class S, class G> const bool operator ==(const property<T, S, G> value) const
			{
				return static_cast<T>(*this) == *value;
			}

			template<typename T, class S, class G> const bool operator !=(const property<T, S, G> value) const
			{
				return static_cast<T>(*this) != *vlaue;
			}

			template<typename T, class S, class G> const bool operator > (const property<T, S, G> value) const
			{
				return static_cast<T>(*this) > *value;
			}

			template<typename T, class S, class G> const bool operator < (const property<T, S, G> value) const
			{
				return static_cast<T>(*this) < *value;
			}

			template<typename T, class S, class G> const bool operator >=(const property<T, S, G> value) const
			{
				return static_cast<T>(*this) >= *value;
			}

			template<typename T, class S, class G> const bool operator <=(const property<T, S, G> value) const
			{
				return static_cast<T>(*this) <= *value;
			}

			template<typename T> const bool operator ==(const nullable__<T> value) const
			{
				return static_cast<T>(*this) == *value;
			}

			template<typename T> const bool operator !=(const nullable__<T> value) const
			{
				return static_cast<T>(*this) != *value;
			}

			template<typename T> const bool operator > (const nullable__<T> value) const
			{
				return static_cast<T>(*this) > *value;
			}

			template<typename T> const bool operator < (const nullable__<T> value) const
			{
				return static_cast<T>(*this) < *value;
			}

			template<typename T> const bool operator >=(const nullable__<T> value) const
			{
				return static_cast<T>(*this) >= *value;
			}

			template<typename T> const bool operator <=(const nullable__<T> value) const
			{
				return static_cast<T>(*this) <= *value;
			}

			template<typename T, class S, class G> const bool operator ==(const nullable_property<T, S, G> value) const
			{
				return static_cast<T>(*this) == *value;
			}

			template<typename T, class S, class G> const bool operator !=(const nullable_property<T, S, G> value) const
			{
				return static_cast<T>(*this) != *vlaue;
			}

			template<typename T, class S, class G> const bool operator > (const nullable_property<T, S, G> value) const
			{
				return static_cast<T>(*this) > *value;
			}

			template<typename T, class S, class G> const bool operator < (const nullable_property<T, S, G> value) const
			{
				return static_cast<T>(*this) < *value;
			}

			template<typename T, class S, class G> const bool operator >=(const nullable_property<T, S, G> value) const
			{
				return static_cast<T>(*this) >= *value;
			}

			template<typename T, class S, class G> const bool operator <=(const nullable_property<T, S, G> value) const
			{
				return static_cast<T>(*this) <= *value;
			}

		private:
			nullable__<std::string> m_value;
		};
	} // namespace Private
} // namespace XML


template<typename T>
const nullable__<T>& nullable_setter(nullable__<T>& lhs, const XML::Private::XString& rhs)
{
	return ::nullable_setter(lhs, nullable__<T>(rhs));
}

template<typename T, class S, class G>
const property<T, S, G>& property_setter(property<T, S, G>& lhs, const XML::Private::XString& rhs)
{
	return ::property_setter(lhs, T(rhs));
}

template<typename T, class S, class G>
const nullable_property<T, S, G>& nullable_property_setter(nullable_property<T, S, G>& lhs, const XML::Private::XString& rhs)
{
	return ::nullable_property_setter(lhs, nullable_property<T, S, G>(rhs));
}

#endif // XML_PRIVATE_XSTRING_INCLUDE_H_