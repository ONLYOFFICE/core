#pragma once
#ifndef XML_XCOMMENT_INCLUDE_H_
#define XML_XCOMMENT_INCLUDE_H_

#include "Private/XPointer.h"
#include "Private/Comment.h"
#include "nullable.h"
#include "property.h"
#include "nullable_property.h"
#include "Private/XString.h"
#include "Private/Lexigraph.h"


namespace XML
{
	class XNode;

	class XComment : public Private::XPointer<Private::Comment>
	{
	public:
		template<typename T>
		explicit XComment(const T& value)
			: base(new Private::Comment(ToString(value))),
				Value(m_ptr->Value)
		{
		}		

		XComment(const XNode& xnode);

	public:
		Private::XString& Value;
	};
} // namespace XML

#endif // XML_XCOMMENT_INCLUDE_H_