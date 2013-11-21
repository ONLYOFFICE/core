#include "stdafx.h"
#include "MSXMLError.h"


namespace EXCEPT
{;
namespace RT
{;

MSXMLError::MSXMLError(const std::string& message)
	: RuntimeException(message)
{
}

} // namespace RT
} // namespace EXCEPT
