#include "stdafx.h"
#include "exceptions.h"


namespace EXCEPT
{;


LogicalException::LogicalException(const std::string& message)
	: logic_error(message)
{
	Log::error("Logical exception!!! " + message);
}


RuntimeException::RuntimeException(const std::string& message)
: runtime_error(message)
{
	Log::error("Run-time exception!!! " + message);
}


} // namespace EXCEPT