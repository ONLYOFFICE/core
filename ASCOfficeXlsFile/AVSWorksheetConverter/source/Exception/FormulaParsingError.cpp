#include "stdafx.h"
#include "FormulaParsingError.h"


namespace EXCEPT
{;
namespace LE
{;

FormulaParsingError::FormulaParsingError(const std::string& message, const std::string& function_name)
: LogicalException(std::string("Formula parsed incorrectly or input data was wrong") + message + " Function: " + function_name)
{
}


} // namespace LE
} // namespace EXCEPT
