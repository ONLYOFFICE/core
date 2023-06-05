#ifndef SM_PARSER_RUNTIMEEXCEPTION_H
#define SM_PARSER_RUNTIMEEXCEPTION_H

#include <exception>
#include "base_types.h"

namespace StarMathParser
{

class RuntimeException : std::exception
{
protected:
	TString m_sDescription;
public:
	RuntimeException(const TString& sError) : m_sDescription{sError}
	{
	}

	const TChar* what() const noexcept override
	{
		return m_sDescription.c_str();
	}
};

}

#endif // SM_PARSER_RUNTIMEEXCEPTION_H
