#pragma once
#ifndef XML_XTEXT_INCLUDE_H_
#define XML_XTEXT_INCLUDE_H_

#include "Private/XPointer.h"
#include "Private/Text.h"
#include "Private/NullText.h"
#include "Private/Null.h"
#include "nullable.h"
#include "property.h"
#include "nullable_property.h"
#include "Private/XString.h"
#include "Private/Lexigraph.h"


namespace XML
{
	class XNode;

	class XText : public Private::XPointer<Private::Text>
	{
	public:
		template<typename T>
		explicit XText(const T& value)
			: base(new Private::Text(ToString(value))),
				Value(m_ptr->Value)
		{
		}

		template<typename T>
		explicit XText(const nullable__<T>& value)
			: base(value.is_init() ? new Private::Text(ToString(value))	: 0),
				Value(value.is_init() ? m_ptr->Value : Private::Null::Text().Value)
		{
		}

		template<typename T, class S, class G>
		explicit XText(const nullable_property<T, S, G>& value)
			: base(value.is_init() ? new Private::Text(ToString(value))	: 0),
				Value(value.is_init() ? m_ptr->Value : Private::Null::Text().Value)
		{
		}

		XText(const XNode& xnode);

	public:
		Private::XString& Value;
	};
} // namespace XML

#endif // XML_XTEXT_INCLUDE_H_