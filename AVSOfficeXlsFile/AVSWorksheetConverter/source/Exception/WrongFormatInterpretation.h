#pragma once

#include "WrongAPIUsage.h"

namespace EXCEPT
{;
namespace LE
{;

class WrongFormatInterpretation : public WrongAPIUsage
{
public:
	explicit WrongFormatInterpretation(const std::string& function_name);
};

} // namespace LE
} // namespace EXCEPT

