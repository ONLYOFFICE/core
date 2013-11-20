#pragma once
#ifndef XML_PRIVATE_NULL_NAMESPACE_INCLUDE_H_
#define XML_PRIVATE_NULL_NAMESPACE_INCLUDE_H_

#include "Namespace.h"


namespace XML
{
	namespace Private
	{
		class NullNamespace : public Namespace
		{
		public:
			virtual const bool exist() const;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_NULL_NAMESPACE_INCLUDE_H_