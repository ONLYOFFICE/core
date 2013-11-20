#pragma once
#ifndef XML_PRIVATE_NULL_ATTRIBUTE_INCLUDE_H_
#define XML_PRIVATE_NULL_ATTRIBUTE_INCLUDE_H_

#include "Attribute.h"


namespace XML
{
	namespace Private
	{
		class NullAttribute : public Attribute
		{
		public:
			virtual const bool exist() const;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_NULL_ATTRIBUTE_INCLUDE_H_