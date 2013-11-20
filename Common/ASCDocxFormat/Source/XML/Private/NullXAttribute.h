#pragma once
#ifndef XML_PRIVATE_NULL_XATTRIBUTE_INCLUDE_H_
#define XML_PRIVATE_NULL_XATTRIBUTE_INCLUDE_H_

#include "./../XAttribute.h"


namespace XML
{
	namespace Private
	{
		class NullXAttribute : public XAttribute
		{
		public:
			NullXAttribute();

		public:
			virtual const bool exist() const;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_NULL_XATTRIBUTE_INCLUDE_H_