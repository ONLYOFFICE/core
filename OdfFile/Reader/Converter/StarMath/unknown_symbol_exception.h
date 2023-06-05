#ifndef SM_PARSER_UNKNOWNSYMBOLEXCEPTION_H
#define SM_PARSER_UNKNOWNSYMBOLEXCEPTION_H

#include <sstream>
#include "runtime_exception.h"

namespace StarMathParser
{

class UnknownSymbolException : public RuntimeException
{
public:
	UnknownSymbolException(const TString& sError, int index)
		: RuntimeException(sError)
	{
		std::stringstream ss;
		ss << " at column: " << index;
		m_sDescription += ss.str();
	}
};

}

#endif // SM_PARSER_UNKNOWNSYMBOLEXCEPTION_H
