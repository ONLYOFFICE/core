#include "stdafx.h"
#include "WrongIntermediateXMLFormat.h"


namespace EXCEPT
{;
namespace LE
{;

WrongIntermediateXMLFormat::WrongIntermediateXMLFormat(const std::string& message, const std::string& function_name)
	: LogicalException(message + " Function: " + function_name)
{
}


} // namespace LE
} // namespace EXCEPT
