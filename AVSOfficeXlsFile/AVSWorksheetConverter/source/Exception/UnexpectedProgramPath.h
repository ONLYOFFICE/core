#pragma once

#include "WrongAPIUsage.h"

namespace EXCEPT
{;
namespace LE
{;

class UnexpectedProgramPath : public WrongAPIUsage
{
public:
	explicit UnexpectedProgramPath(const std::string& message, const std::string& function_name);
};

typedef UnexpectedProgramPath WhatIsTheFuck;
typedef UnexpectedProgramPath HowCouldItHappen;

} // namespace LE
} // namespace EXCEPT

