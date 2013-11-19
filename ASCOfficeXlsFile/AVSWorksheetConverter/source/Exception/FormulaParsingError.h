#pragma once

#include "exceptions.h"

namespace EXCEPT
{;
namespace LE
{;

class FormulaParsingError : public LogicalException
{
public:
	explicit FormulaParsingError(const std::string& message, const std::string& function_name);
};


} // namespace LE
} // namespace EXCEPT

