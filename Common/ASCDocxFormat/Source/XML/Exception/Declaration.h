#pragma once
#ifndef XML_EXCEPTION_DECLARATION_INCLUDE_H_
#define XML_EXCEPTION_DECLARATION_INCLUDE_H_

#include "Base.h"


namespace XML
{
	namespace Exception
	{
		class Declaration : public Base
		{
		public:
			Declaration(const std::string& message);
		};
	} // namespace Exception
} // namespace XML

#endif // XML_EXCEPTION_DECLARATION_INCLUDE_H_