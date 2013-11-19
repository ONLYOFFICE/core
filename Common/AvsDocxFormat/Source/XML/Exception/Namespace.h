#pragma once
#ifndef XML_EXCEPTION_NAMESPACE_INCLUDE_H_
#define XML_EXCEPTION_NAMESPACE_INCLUDE_H_

#include "Base.h"


namespace XML
{
	namespace Exception
	{
		class Namespace : public Base
		{
		public:
			Namespace(const std::string& message);
		};
	} // namespace Exception
} // namespace XML

#endif // XML_EXCEPTION_NAMESPACE_INCLUDE_H_