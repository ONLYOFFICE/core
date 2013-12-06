#pragma once
#ifndef XML_EXCEPTION_BASE_INCLUDE_H_
#define XML_EXCEPTION_BASE_INCLUDE_H_

#include "Exception/log_invalid_argument.h"
#include <string>

namespace XML
{
	namespace Exception
	{
		class Base : public log_invalid_argument
		{
		public:
			Base(const std::string& message);
		};
	} // namespace Exception
} // namespace XML

#endif // XML_EXCEPTION_BASE_INCLUDE_H_