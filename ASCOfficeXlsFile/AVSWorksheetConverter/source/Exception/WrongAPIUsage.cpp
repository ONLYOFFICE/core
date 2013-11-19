#include "stdafx.h"
#include "WrongAPIUsage.h"


namespace EXCEPT
{;
namespace LE
{;

WrongAPIUsage::WrongAPIUsage(const std::string& message, const std::string& function_name)
	: LogicalException(message + " Function: " + function_name)
{
}


} // namespace LE
} // namespace EXCEPT
