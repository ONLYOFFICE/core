#pragma once

#include "exceptions.h"

namespace EXCEPT
{;
namespace LE
{;

class WrongIntermediateXMLFormat : public LogicalException
{
public:
	explicit WrongIntermediateXMLFormat(const std::string& message, const std::string& function_name);
};


} // namespace LE
} // namespace EXCEPT

