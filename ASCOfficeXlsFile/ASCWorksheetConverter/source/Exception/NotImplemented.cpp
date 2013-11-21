#include "stdafx.h"
#include "NotImplemented.h"


namespace EXCEPT
{;
namespace LE
{;

NotImplemented::NotImplemented(const std::string& function_name)
	: UnexpectedProgramPath("The functionality is not implemented.", function_name)
{
}

} // namespace LE
} // namespace EXCEPT
