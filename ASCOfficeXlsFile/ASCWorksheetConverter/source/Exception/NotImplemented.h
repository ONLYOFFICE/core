#pragma once

#include "UnexpectedProgramPath.h"

namespace EXCEPT
{;
namespace LE
{;

class NotImplemented: public UnexpectedProgramPath
{
public:
	explicit NotImplemented(const std::string& function_name);
};

} // namespace LE
} // namespace EXCEPT

