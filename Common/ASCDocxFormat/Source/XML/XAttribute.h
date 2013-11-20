#pragma once
#ifndef XML_XATTRIBUTE_INCLUDE_H_
#define XML_XATTRIBUTE_INNLUDE_H_

#include "Private/XPointer.h"
#include "Private/Attribute.h"
#include "Private/XString.h"

namespace XML
{
	namespace Private
	{
		class NullAttribute;
	}

	class XAttribute : public Private::XPointer<Private::Attribute>
	{
	public:
		XAttribute();

		template<typename Type>
		XAttribute(const XName& name, const Type& value)
			: base(new Private::Attribute(name, ToString(value)))
		{
		}

		template<typename Type>
		XAttribute(const XName& name, const nullable<Type>& value)
			: base(value.is_init() ? new Private::Attribute(name, ToString(value)) : 0)
		{
		}

		template<typename Type, class S, class G>
		XAttribute(const XName& name, const nullable_property<Type, S, G>& value)
			: base(value.is_init() ? new Private::Attribute(name, ToString(value)) : 0)
		{
		}

	protected:
		XAttribute(Private::NullAttribute);

	public:
		virtual const bool exist() const;
		virtual const Private::XString value() const;
		virtual const Private::XString get_value_or(const std::string& def)const;
		const bool operator ==(const XAttribute& rhs) const;
		const bool operator <(const XAttribute& rhs) const;

	};
} // namespace XML

#endif // XML_XATTRIBUTE_INCLUDE_H_