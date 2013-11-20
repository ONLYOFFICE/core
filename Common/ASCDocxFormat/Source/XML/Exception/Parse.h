#pragma once
#ifndef XML_EXCEPTION_PARSE_INCLUDE_H_
#define XML_EXCEPTION_PARSE_INCLUDE_H_

#include "Base.h"


namespace XML
{
	namespace Exception
	{
		class Parse : public Base
		{
		public:
			Parse(const std::string& message);
		};
	} // namespace Exception
} // namespace XML

#endif // XML_EXCEPTION_PARSE_INCLUDE_H_