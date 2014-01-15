#pragma once
#ifndef XML_PRIVATE_NULL_XNAMESPACE_INCLUDE_H_
#define XML_PRIVATE_NULL_XNAMESPACE_INCLUDE_H_

#include "./../XNamespace.h"


namespace XML
{
	namespace Private
	{
		class NullXNamespace : public XNamespace
		{
			virtual const bool exist() const;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_NULL_XNAMESPACE_INCLUDE_H_