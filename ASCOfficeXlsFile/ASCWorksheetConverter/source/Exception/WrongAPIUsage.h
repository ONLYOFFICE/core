#pragma once

#include "exceptions.h"

namespace EXCEPT
{;
namespace LE
{;

class WrongAPIUsage : public LogicalException
{
public:
	explicit WrongAPIUsage(const std::string& message, const std::string& function_name);
};


} // namespace LE
} // namespace EXCEPT

