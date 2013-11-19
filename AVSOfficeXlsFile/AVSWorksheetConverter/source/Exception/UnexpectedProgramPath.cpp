#include "stdafx.h"
#include "UnexpectedProgramPath.h"


namespace EXCEPT
{;
namespace LE
{;

UnexpectedProgramPath::UnexpectedProgramPath(const std::string& message, const std::string& function_name)
	: WrongAPIUsage(message, function_name)
{
}

} // namespace LE
} // namespace EXCEPT
