#pragma once
#ifndef XML_PRIVATE_NULL_ELEMENT_INCLUDE_H_
#define XML_PRIVATE_NULL_ELEMENT_INCLUDE_H_


#include "Element.h"


namespace XML
{
	namespace Private
	{
		class NullElement : public Element
		{
		public:
			virtual const bool exist() const;
			virtual const XString text() const;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_NULL_ELEMENT_INCLUDE_H_